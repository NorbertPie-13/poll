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

    char buf[BUFF_SZ] = { 0 };    // Buffer for client data

    int fd_count = 0;
    int fd_size = SIZE;
    struct pollfd *pfds = calloc(fd_size, sizeof(*pfds));

    int listener = unix_server_setup(argv[1]);
    if (ERROR == listener)
    {
        goto DESTROY_POLL;
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN;

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
            { 

                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection

                    int newfd = 0;
                    if (FAIL != unix_accept(listener, &newfd))
                    {
                        if (FAIL == poll_server_add(&pfds, newfd, &fd_count, &fd_size))
                        {
                            goto CLOSE;
                        }
                    }
                    
                    printf("Connection from socket: %d\n", newfd);
                }
                else
                {
                    
                    int nbytes = unix_recv_data(pfds[i].fd, buf, 16);

                    int sender_fd = pfds[i].fd;
                    if ( 0 >= nbytes)
                    {

                        if (0 == nbytes)
                        {
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(pfds[i].fd);
                        poll_remove(pfds, i, &fd_count);

                    }
                    else
                    {
                        response_t return_code = { 0 };

                        file_header_t header = { 0 };

                        memmove(&header, buf, 8);
                        
                        printf("File name size: %d\n", header.file_name_length);
                        printf("File length: %d\n", header.file_length);

                        if (0 < header.file_length)
                        {
                            return_code.return_code = SUCCESS;
                        }
                        else
                        {
                            return_code.return_code = INOP;
                        }

                        size_t size = sizeof(response_t);

                        unix_send_data(sender_fd, &return_code, &size);
                        memset(buf, 0, sizeof(buf));

                    }
                }
            }
        }
    }

CLOSE:
    close(pfds[0].fd);

    ret_val = PASS;
DESTROY_POLL:
    free(pfds);
    pfds = NULL;
    
EXIT:
    return ret_val;
}

/* End of File */
