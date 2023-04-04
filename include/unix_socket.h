#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "err_funcs.h"

#define FAIL 1
#define PASS 0
#define ERROR -1
#define MAX_PATH_LEN 108
#define BACKLOG 10

int unix_server_setup(char *p_file);

/**
 * @brief Create a unix socket object
 * 
 * @param p_container Container for File Descriptor
 * @return int Return 1 on error, or 0 on success
 */
int create_unix_socket(int * p_container);

int unix_bind(char * p_path, int *socket);

int unix_accept(int socket, int * p_container);


#endif