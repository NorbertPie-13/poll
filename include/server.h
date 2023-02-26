/**
 * @file server.h
 * @author CW2 Matthew N. Pieczynski (matthew.n.pieczynski.mil@army.mil)
 * @brief A library to be used to set up a TCP/IP socket with IPV4.
 * @version 0.1
 * @date 2023-01-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include <sys/socket.h>
#include <netdb.h>

/**
 * @brief Get host information for server socket.
 *
 * @param pp_server_info Pointer to host options
 * @param p_port p_arg is the port to be used
 * @return Return 1 on error, 0 on success.
 */
int server_get_host_info(struct addrinfo **pp_server_info, char *p_port);

/**
 * @brief Bind a socket with the Host system socket options.
 * @param sockfd Pointer to socket file descriptor
 * @param p_addr_info Pointer to the addr information
 * @return Return 1 on failure and 0 on sucess.
 */
int server_bind_socket(int *p_sockfd, struct addrinfo *p_addr_info);

/**
 * @brief Create a socket object. Return 1 on failure and 0 on success.
 *
 * @param p_sockfd
 * @param server_info
 * @return int
 */
int server_create_socket(int *p_sockfd, struct addrinfo **pp_server_info);

/**
 * @brief Set the socket opts object.
 *
 * @param sockfd socket file descriptor
 * @param opts
 * @return int Return 1 on failure and 0 on sucess.
 */
int server_set_socket_opts(int *p_sockfd, int *p_opts);

/**
 * @brief Pass socked file descriptor and begin listening on the socket for
 * possible connections
 *
 * @param sockfd
 * @return int return 1 on erro, 0 on success
 */
int server_listen_for_socket(int *p_sockfd);

/**
 * @brief Find if client address is IPV4 or IPV6
 *
 * @param client Client addr structure
 * @return Return NULL on failure, or return IPV6/ IPV4 struct.
 */
void *server_get_addr(struct sockaddr *p_client);

/**
 * @brief Set socket permissions to Non-blocking
 * 
 * @param p_sockfd Socket file descriptor
 * @return int Return 1 on error, 0 on success
 */
int server_set_non_block(int * p_sockfd);

/**
 * @brief Set up server
 * 
 * @param p_port Port to be used.
 * @return int Return -1 on error, otherwise return file descriptor to server.
 */
int server_setup(char *p_port);

/**
 * @brief Cited from "https://beej.us/guide/bgnet/html/#sendall".
 * Function is along the same lines with some additional checks.
 * 
 * @param sockfd Client socket
 * @param buffer Buffer to send
 * @param length The size of the buffer
 * @return int Return -1 on error or 0 on Success.
 */
int server_send_data(int sockfd, char * buffer, int *buffer_sz);

/**
 * @brief Loop and handle partial recvs
 * 
 * @param sockfd socket to recv from.
 * @param buffer Buffer to store in.
 * @param buffer_sz How much is expected to store.
 * @return int 
 */
int server_recv_data(int sockfd, char * buffer, int buffer_sz);

/**
 * @brief Wrapper to accept a client socket
 * 
 * @param sockfd Server socket
 * @return int Return -1 on Error, otherwise return socket file descriptor.
 */
int server_accept(int sockfd);

#endif