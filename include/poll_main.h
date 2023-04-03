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
#include "server.h"
#include "poll_server.h"
#include "err_funcs.h"
#include <signal.h>

#define ARGS 2
#define FAIL 1
#define PASS 0


#endif