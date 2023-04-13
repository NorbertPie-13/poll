#ifndef POLL_SERVER_H
#define POLL_SERVER_H

#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DOUBLE 2
#define NEW_STRUCT 1

/**
 * @brief Remove a file descriptor from Poll array.
 * 
 * @param poll_structs Array of poll structures
 * @param index The index to remove
 * @param poll_count The current count for poll structures
 * @return int Return 1 on error, or 0 on success
 */
int poll_remove(struct pollfd *poll_structs, int index, int * poll_count);

/**
 * @brief Add a poll structure to the array of poll structures
 * 
 * @param poll_structs The array of poll structures
 * @param client_fd The file descriptor to add
 * @param poll_count The current size of the poll arrayd
 * @param max_size The current maximum size of the array
 * @return int Return 1 on error, 0 on success
 */
int poll_server_add(struct pollfd **poll_structs, int client_fd, int* poll_count, int * max_size);


#endif