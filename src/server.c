#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include "err_funcs.h"
#include "server.h"

#define BACKLOG 100
#define TIMEOUT 20

extern volatile sig_atomic_t running;


/**
 * @brief Get address from client address
 * 
 * @param p_client 
 * @return void* return 
 */
void *
server_get_addr(struct sockaddr *p_client)
{
    void *value = p_client;
    if (NULL == p_client)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    if (AF_INET == p_client->sa_family)
    {
        value = &(((struct sockaddr_in *)p_client)->sin_addr);
    }
    else
    {
        value = &(((struct sockaddr_in6 *)p_client)->sin6_addr);
    }
EXIT:
    return value;
}

/**
 * @brief Get host information for  server to use.
 * 
 * @param pp_server_info 
 * @param p_port 
 * @return int 
 */
int
server_get_host_info(struct addrinfo **pp_server_info, char *p_port)
{
    int ret_val = EXIT_FAILURE;
    if ((NULL == pp_server_info) || (NULL == p_port))
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    // declare socket options struct and zero out.
    struct addrinfo hints = { 0 };

    // Populate options with IPV4, TCP, and AI_Passive
    hints.ai_family   = AF_UNSPEC;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
    int error_code    = EXIT_FAILURE;
    // Set socket options for the server and set the file descriptor
    if (0 != (error_code = getaddrinfo(NULL, p_port, &hints, pp_server_info)))
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error_code));
        goto EXIT;
    }
    ret_val = EXIT_SUCCESS;
EXIT:
    return ret_val;
}

/**
 * @brief Wrapper to accept a client socket
 * 
 * @param sockfd Server socket
 * @return int Return -1 on Error, otherwise return socket file descriptor.
 */
int server_accept(int sockfd)
{
    int client_fd = -1;
    if (-1 == sockfd)
    {
        perror("Bad Server Socket");
        goto EXIT;
    }
    struct sockaddr_storage client_addr = { 0 };
    char ipv6[INET6_ADDRSTRLEN] = { 0 };
    socklen_t client_sz  = sizeof(client_addr);    
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_sz);
    if (-1 == client_fd)
    {
        perror("accept");
        goto EXIT;
    }
    inet_ntop(client_addr.ss_family,
            server_get_addr((struct sockaddr *)&client_addr),
            ipv6,
            sizeof(ipv6));
    printf("Server: Connection from %s\n", ipv6);
    struct timeval timer = { 0 };
    timer.tv_sec = TIMEOUT;

    errno = 0;
    if (-1 == setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timer, sizeof(timer)))
    {
        perror("Setsockopt");
        close(client_fd);
        client_fd = -1;
    }

    EXIT:
        return client_fd;
}

/**
 * @brief Create a server socket.
 * 
 * @param p_sockfd A pointer to store the socket.
 * @param pp_server_info A reference to an addrinfo structure
 * @return int Return 1 on Error, 0 on success.
 */
int
server_create_socket(int *p_sockfd, struct addrinfo **pp_server_info)
{
    int ret_val = EXIT_FAILURE;
    // check pointers passed
    if ((NULL == p_sockfd) || (NULL == pp_server_info))
    {
        goto PRINT_ERROR_EXIT;
    }
    
    // create temp variable
    struct addrinfo *temp = *pp_server_info;
    if (NULL == temp)
    {
        goto PRINT_ERROR_EXIT;
    }
    // create socket with the temp varibles type and protocol
    *p_sockfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
    if (-1 == err_funcs_chk(*p_sockfd, "Server: socket"))
    {
        goto EXIT;
    }
    ret_val = EXIT_SUCCESS;
    goto EXIT;
PRINT_ERROR_EXIT:
    err_funcs_print_error(__func__);

EXIT:
    return ret_val;
}

/**
 * @brief Set the socket options for the server.
 * 
 * @param p_sockfd Reference to socket
 * @param p_opts Options
 * @return int return 1 on error, 0 on success
 */
int
server_set_socket_opts(int *p_sockfd, int *p_opts)
{
    int ret_val = EXIT_FAILURE;
    // check pointers passed into function
    if ((NULL == p_sockfd) || (NULL == p_opts))
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    
    // set the socket options to use socket level and to reuse the address
    int func_val
        = setsockopt(*p_sockfd, SOL_SOCKET, SO_REUSEADDR, &p_opts, sizeof(int));
        

    if (-1 == err_funcs_chk(func_val, "Setsocketopt"))
    {
        goto EXIT;
    }

    ret_val = EXIT_SUCCESS;
EXIT:
    return ret_val;
}

/**
 * @brief Bind a socket to an address
 * 
 * @param p_sockfd Reference to socket  
 * @param p_addr_info reference to addrinfo structure   
 * @return int Return 1 on error, 0 on success.
 */
int
server_bind_socket(int *p_sockfd, struct addrinfo *p_addr_info)
{
    int ret_val = EXIT_FAILURE;
    int func_val = EXIT_FAILURE;
    if ((NULL == p_sockfd) || (NULL == p_addr_info))
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    
    func_val     = bind(*p_sockfd, p_addr_info->ai_addr, p_addr_info->ai_addrlen);
    if (-1 == err_funcs_chk(func_val, "Server: bind"))
    {
        goto EXIT;
    }
    ret_val = EXIT_SUCCESS;
EXIT:
    return ret_val;
}

/**
 * @brief Set up listen for socket.
 * 
 * @param p_sockfd Reference to socket to listen on.
 * @return int Return 1 on error, 0 on success
 */
int
server_listen_for_socket(int *p_sockfd)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == p_sockfd)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    if (-1 == err_funcs_chk(listen(*p_sockfd, BACKLOG), "listen"))
    {
        goto EXIT;
    }

    ret_val = EXIT_SUCCESS;
EXIT:
    return ret_val;
}

/**
 * @brief Set a socket to non-block.
 * 
 * @param p_sockfd Reference to socket  
 * @return int Return 1 on error, 0 on success
 */
int
server_set_non_block(int *p_sockfd)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == p_sockfd)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    if (!(fcntl(*p_sockfd, F_GETFL) & O_NONBLOCK))
    {
        fcntl(*p_sockfd, F_SETFL, fcntl(*p_sockfd, F_GETFL) | O_NONBLOCK);
    }
    ret_val = EXIT_SUCCESS;
EXIT:
    return ret_val;
}

/**
 * @brief Set up server
 * 
 * @param p_port Port to be used.
 * @return int Return -1 on error, otherwise return file descriptor to server.
 */
int server_setup(char *p_port)
{
    int sockfd = -1;
    struct addrinfo *server_info = NULL;
    int opt = 1;
    if (NULL == p_port)
    {
        goto EXIT;
    }

    printf("Socket established...\n");
    if (-1 == server_get_host_info(&server_info, p_port))
    {
        goto EXIT;
    }
    struct addrinfo *p_addr_info;
    // loop throught addresses until finding an address that is not NULL;
    for (p_addr_info = server_info; NULL != p_addr_info;
         p_addr_info = p_addr_info->ai_next)
    {
        if (1 == server_create_socket(&sockfd, &p_addr_info))
        {
            goto FREE_ADDR;
        }

        if (1 == server_set_socket_opts(&sockfd, &opt))
        {
            goto CLOSE_SOCKET;
        }

        if (1 == server_bind_socket(&sockfd, p_addr_info))
        {
            goto CLOSE_SOCKET;
        }
        break;
    }

    if (EXIT_FAILURE == server_listen_for_socket(&sockfd))
    {
        goto CLOSE_SOCKET;
    }
    printf("Waiting for connection....\n");
    goto FREE_ADDR;

    CLOSE_SOCKET:
        close(sockfd);
        sockfd = -1;

    FREE_ADDR:
    freeaddrinfo(server_info);

    EXIT:
        return sockfd;
}

/**
 * @brief Cited from "https://beej.us/guide/bgnet/html/#sendall".
 * Function is along the same lines with some additional checks.
 * 
 * @param sockfd Client socket
 * @param buffer Buffer to send
 * @param length The size of the buffer
 * @return int Return -1 on error or 0 on Success.
 */
int server_send_data(int sockfd, char * buffer, int *buffer_sz)
{
    int ret_val = -1;
    int total_bytes_sent = 0;
    int bytes_remaining = *buffer_sz;
    int bytes_sent = 0;
    if ((-1 == sockfd) || (NULL == buffer) || (NULL == buffer_sz))
    {
        if (-1 == sockfd)
        {
            fprintf(stderr, "Socket is closed.\n");
        } else {
            err_funcs_print_error(__func__);
        }
        goto EXIT;
    }
    while (total_bytes_sent < *buffer_sz)
    {
        bytes_sent = send(sockfd, buffer + total_bytes_sent, bytes_remaining, 0);
        if (-1 == bytes_sent)
        {
            perror("send");
            goto EXIT;
        }
        total_bytes_sent += bytes_sent;
        bytes_remaining -= bytes_sent;
    }
    *buffer_sz = total_bytes_sent;

    ret_val = 0;
    EXIT:
        return ret_val;
}

/**
 * @brief Loop and handle partial recvs
 * 
 * @param sockfd socket to recv from.
 * @param buffer Buffer to store in.
 * @param buffer_sz How much is expected to store.
 * @return int 
 */
int server_recv_data(int sockfd, char * buffer, int buffer_sz)
{
    int ret_val = -1;
    ssize_t bytes_recv = 0;
    ssize_t total_bytes_recv = 0;
    int bytes_remaining = buffer_sz;
    if ((-1 == sockfd) || (NULL == buffer))
    {
        if (-1 == sockfd)
        {
            fprintf(stderr, "Socket is closed.\n");
        } else {
            err_funcs_print_error(__func__);
        }
        goto EXIT;
    }

    for(;;)
    {
        if (1 == running)
        {
            goto EXIT;
        }
        if (0 == bytes_remaining)
        {
            break;
        }
        bytes_recv = recv(sockfd, buffer + total_bytes_recv, 1, 0);
        if (0 >= bytes_recv)
        {
            
            if (0 == bytes_recv)
            {
                goto EXIT_COMPLETE;
            } else {
                goto EXIT;
            }
        }
        
        total_bytes_recv += bytes_recv;
        bytes_remaining -= bytes_recv;
    }

    EXIT_COMPLETE:
        ret_val = total_bytes_recv;
    EXIT:
        errno = 0;
        return ret_val;
}


