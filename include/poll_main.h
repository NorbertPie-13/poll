#ifndef POLL_MAIN_H
#define POLL_MAIN_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <signal.h>
#include "server.h"
#include "poll_server.h"
#include "err_funcs.h"
#include "unix_socket.h"


#define ARGS 2
#define FAIL 1
#define PASS 0
#define BUFF_SZ 256
#define SIZE 2


typedef struct
{
    int32_t len;
} custom_header_t;


#endif