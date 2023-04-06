#include "unix_socket.h"

static int check_path(char * p_path, struct sockaddr_un * p_addr);

extern volatile sig_atomic_t running;

/**
 * @brief Set up server
 * 
 * @param p_port Port to be used.
 * @return int Return -1 on error, otherwise return file descriptor to server.
 */
int unix_server_setup(char *p_file)
{
    int sockfd = ERROR;
    if (NULL == p_file)
    {
        goto EXIT;
    }

    printf("Socket established...\n");

    if (FAIL == create_unix_socket(&sockfd))
    {
        goto EXIT;
    }

    if (FAIL == unix_bind(p_file, &sockfd))
    {
        goto CLOSE_SOCKET;
    }
    if (ERROR == listen(sockfd, BACKLOG))
    {
        perror("listen");
        goto CLOSE_SOCKET;
    }

    printf("Waiting for connection....\n");
    goto EXIT;

    CLOSE_SOCKET:
        close(sockfd);
        sockfd = ERROR;

    EXIT:
        return sockfd;
}

int unix_bind(char * p_path, int *socket)
{
    int ret_val = FAIL;
    struct sockaddr_un server_info = { 0 };
    server_info.sun_family = AF_UNIX;

    if (NULL == p_path)
    {
        fprintf(stderr, "%s recieved NULL.\n", __func__);
        goto EXIT;
    }
    if (FAIL == check_path(p_path, &server_info))
    {
        goto EXIT;
    }
    strncpy(server_info.sun_path, p_path, MAX_PATH_LEN);
    unlink(server_info.sun_path);
    int address_len =  strlen(server_info.sun_path) +  sizeof(server_info.sun_family);
    if (ERROR == bind(*socket, (struct sockaddr *)&server_info, address_len))
    {
        perror("Bind");
        goto EXIT;
    }

    ret_val = PASS;
EXIT:
    return ret_val;
}

int create_unix_socket(int * p_container)
{
    int ret_val = FAIL;
    if (NULL == p_container)
    {
        fprintf(stderr, "%s recieved NULL pointer.\n", __func__);
        goto EXIT;
    }

    errno = 0;
    *p_container = socket(AF_UNIX, SOCK_STREAM, 0);
    if (ERROR == *p_container)
    {
        perror("Socket");
        errno = 0;
        goto EXIT;
    }

    ret_val = PASS;
EXIT:
    return ret_val;
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
int unix_send_data(int sockfd, char * buffer, int *buffer_sz)
{
    int ret_val = -1;
    int total_bytes_sent = 0;
    int bytes_remaining = *buffer_sz;
    int bytes_sent = 0;
    if (-1 == sockfd)
    {
        fprintf(stderr, "Socket is closed.\n");
        goto EXIT;
    } 

    if ((NULL == buffer) || (NULL == buffer_sz))
    {
        err_funcs_print_error(__func__);
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
int unix_recv_data(int sockfd, char * buffer, int buffer_sz)
{
    int ret_val = -1;
    ssize_t bytes_recv = 0;
    ssize_t total_bytes_recv = 0;
    int bytes_remaining = buffer_sz;
    if (-1 == sockfd) 
    
    {
        fprintf(stderr, "Socket is closed.\n");
        goto EXIT;
    }
    if (NULL == buffer)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }

    while (total_bytes_recv < bytes_remaining)
    {
        // Running is an external variable used in the threading case.
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
                perror("Recv");
                errno = 0;
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

int unix_accept(int socket, int * p_container)
{
    int ret_val = FAIL;
    if ((0 >= socket) || (NULL == p_container))
    {
        fprintf(stderr, "%s recieved bad parameters.\n", __func__);
        goto EXIT;
    }

    struct sockaddr_un client = { 0 };
    client.sun_family = AF_UNIX;

    socklen_t client_len = sizeof(client);

    int temp_client = accept(socket, (struct sockaddr *)&client, &client_len);
    if (ERROR == temp_client)
    {
        perror("Accept");
        goto EXIT;
    }
    
    *p_container = temp_client;
    ret_val = PASS;

EXIT:
    return ret_val;
}

static int check_path(char * p_path, struct sockaddr_un * p_addr)
{
    int ret_val = FAIL;
    if ((NULL == p_path) || (NULL == p_addr))
    {
        fprintf(stderr, "%s recieved NULL pointer.\n", __func__);
        goto EXIT;
    }

    if (sizeof((*p_addr).sun_path) - 1 < strlen(p_path))
    {
        fprintf(stderr, "Path is too long for UNIX socket struct.\n");
        goto EXIT;
    }

    ret_val = PASS;
EXIT:
    return ret_val;
}

/* End of File*/
