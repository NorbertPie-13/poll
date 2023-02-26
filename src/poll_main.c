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

volatile sig_atomic_t running = 0;

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}


void
signal_handler(int signo)
{
    if (SIGINT == signo)
    {
        running = 1;
    }
}

// Main
int main(int argc, char ** argv)
{
    int ret_val = FAIL;
    if (ARGS != argc)
    {
        printf("Not correct arguments\n");
        goto EXIT;
    }
    struct sigaction signals;
    sigemptyset(&signals.sa_mask);
    signals.sa_flags   = SA_RESTART;
    signals.sa_handler = signal_handler;
    if (-1 == sigaction(SIGINT, &signals, NULL))
    {
        perror("sigaction");
        goto EXIT;
    }
     // Newly accept()ed socket descriptor

    char buf[256];    // Buffer for client data

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 2;
    struct pollfd *pfds = calloc(fd_size, sizeof(*pfds));

    // Set up and get a listening socket
    if (FAIL == err_funcs_check_num_string(argv[1]))
    {
        goto DESTROY_POLL;
    }
    int listener = server_setup(argv[1]);
    if (-1 == listener) {
        fprintf(stderr, "error getting listening socket\n");
        goto DESTROY_POLL;
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    while (0 == running)
    {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1) {
            perror("poll");
            break;
        }
        
        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!

                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection

                    int newfd = server_accept(listener);

                    if (newfd == -1)
                    {
                        perror("accept");
                    } else {
                        if (FAIL == poll_server_add(&pfds, newfd, &fd_count, &fd_size))
                        {
                            goto DESTROY_POLL;
                        }
                    }
                } else {
                    // If not the listener, we're just a regular clien
                    int nbytes = server_recv_data(pfds[i].fd, buf, sizeof(buf));
                    // printf("%d bytes from %d\n", nbytes, pfds[i].fd);
                    

                    int sender_fd = pfds[i].fd;
                    if ( 0 >= nbytes)
                    {
                        // Got error or connection closed by client
                        if (0 == nbytes)
                        {
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        } else {
                            perror("recv");
                        }
                        close(pfds[i].fd); // Bye!
                        poll_remove(pfds, i, &fd_count);

                    } else {
                        
                        for (int i = 0; i < fd_count; i++)
                        {
                            int dest = pfds[i].fd;

                            if (dest != listener && dest != sender_fd)
                            {
                                send(dest, buf, sizeof(buf), 0);
                            }

                        }
                        // We got some good data from a client
                        
                        memset(buf, 0, sizeof(buf));
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    ret_val = PASS;
    DESTROY_POLL:
        free(pfds);
        
    EXIT:
        return ret_val;
}