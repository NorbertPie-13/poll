#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include "err_funcs.h"


#define ERROR -1
#define MAX_PATH_LEN 108
#define BACKLOG 10

/**
 * @brief A wrapper function to start a UNIX domain server
 * 
 * @param p_file The file to bind to.
 * @return int Return -1 on Error, or return a socket on success.
 */
int unix_server_setup(char *p_file);

/**
 * @brief Create a unix socket object
 * 
 * @param p_container Container for File Descriptor
 * @return int Return 1 on error, or 0 on success
 */
int create_unix_socket(int * p_container);

/**
 * @brief Bind a socket to a File.
 * 
 * @param p_path File to bind to.
 * @param socket Socket to bind
 * @return int Return 1 on error, 0 on success.
 */
int unix_bind(char * p_path, int *socket);

/**
 * @brief Accept a client connection
 * 
 * @param socket Server socket
 * @param p_container Container for the client socket
 * @return int Return 1 on error, 0 on success
 */
int unix_accept(int socket, int * p_container);

/**
 * @brief Loop and handle partial recvs
 * 
 * @param sockfd socket to recv from.
 * @param buffer Buffer to store in.
 * @param buffer_sz How much is expected to store.
 * @return int 
 */
int unix_recv_data(int sockfd, char * buffer, int bytes_expect);

/**
 * @brief Cited from "https://beej.us/guide/bgnet/html/#sendall".
 * Function is along the same lines with some additional checks.
 * 
 * @param sockfd Client socket
 * @param buffer Buffer to send
 * @param length The size of the buffer
 * @return int Return -1 on error or 0 on Success.
 */
int unix_send_data(int sockfd, void * p_data, size_t *buffer_sz);


#endif