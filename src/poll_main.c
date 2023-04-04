#include "poll_main.h"

volatile sig_atomic_t running = 0;

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
        fprintf(stderr, "./<executable path> <file>\n");
        goto EXIT;
    }
    struct sigaction signals = { 0 };
    sigemptyset(&signals.sa_mask);
    signals.sa_flags   = SA_RESTART;
    signals.sa_handler = signal_handler;
    if (-1 == sigaction(SIGINT, &signals, NULL))
    {
        perror("sigaction");
        goto EXIT;
    }
     // Newly accept()ed socket descriptor

    char buf[BUFF_SZ] = { 0 };    // Buffer for client data

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = SIZE;
    struct pollfd *pfds = calloc(fd_size, sizeof(*pfds));

    // Set up and get a listening socket
    
    int listener = unix_server_setup(argv[1]);
    if (ERROR == listener)
    {
        goto DESTROY_POLL;
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

     // For the listener
    ++fd_count;

    while (0 == running)
    {
        int poll_count = poll(pfds, fd_count, ERROR);

        if (poll_count == ERROR) {
            perror("poll");
            break;
        }
        
        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { // We got one!!

                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection

                    int newfd = 0;
                    if (FAIL != unix_accept(listener, &newfd))
                    {
                        if (FAIL == poll_server_add(&pfds, newfd, &fd_count, &fd_size))
                        {
                            goto DESTROY_POLL;
                        }
                    }
                    
                    printf("Connection from socket: %d\n", newfd);
                }
                else
                {
                    
                    // If not the listener, we're just a regular clien
                    server_recv_data(pfds[i].fd, buf, 8);

                    custom_header_t * incoming = (custom_header_t *) buf;
                    int32_t size = incoming->len;
                    printf("Incoming: %d\n", size);
                    int nbytes = server_recv_data(pfds[i].fd, buf, size);
                    printf("%d bytes from %d\n", nbytes, pfds[i].fd);
                    printf("Buffer>> %s", buf);
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
                                send(pfds[i].fd, buf, sizeof(buf), 0);
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
        pfds = NULL;
        
    EXIT:
        return ret_val;
}

/* End of File */
