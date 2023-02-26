#ifndef POLL_SERVER_H
#define POLL_SERVER_H
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>

int poll_remove(struct pollfd *poll_structs, int index, int * poll_count);

int poll_server_add(struct pollfd **poll_structs, int client_fd, int* poll_count, int * max_size);


#endif