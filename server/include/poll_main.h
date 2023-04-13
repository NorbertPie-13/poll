/**
 * @file poll_main.h
 * @author Matthew Pieczynski
 * @brief A simple unix domain socket that recieves the the length of a file name and the length of a file.
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef POLL_MAIN_H
#define POLL_MAIN_H

#define _GNU_SOURCE


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "poll_server.h"
#include "err_funcs.h"
#include "unix_socket.h"
#include "common.h"


#define ARGS 2
#define SIZE 2
#define BUFF_SZ 256


/**
 * @brief Enumeration for return opcodes for the client
 * 
 */
enum opcodes
{
    SUCCESS,
    INOP
};

#endif