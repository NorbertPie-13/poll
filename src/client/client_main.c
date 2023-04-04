/**
 * @file client_main.c
 * @author Matthew N. Pieczynski
 * @brief Cited from Beej's Unix Interprocess Communication
 * https://beej.us/guide/bgipc/html/split/unixsock.html#unixsock
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SOCK_PATH "echo_socket"

int main(int argc, char ** argv)
{
    int ret_val = EXIT_FAILURE;
    if (2 != argc)
    {
        fprintf(stderr, "./<executable> <file>\n");
        goto EXIT;
    }
    int client = -1;
    int len = 0;
    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
        // .sun_path = SOCK_PATH,   // Can't do assignment to an array
    };
    

    if ((client = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    strcpy(remote.sun_path, argv[1]);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (-1 == connect(client, (struct sockaddr *)&remote, len))
    {
        perror("connect");
        goto EXIT;
    }

    printf("Connected.\n");

    /* size in fgets() includes the null byte */
    for (;;)
    {
        char str[256] = { 0 };
        printf("> ");
        fgets(str, sizeof(str), stdin);
        char fake_header[10] = { 0 };
        size_t len = strlen(str);
        memmove(fake_header, &len, sizeof(size_t));


        if (send(client, fake_header, 8, 0) == -1)
        {
            perror("send");
            goto CLOSE;
        }

        if (send(client, str, strlen(str), 0) == -1)
        {
            perror("send");
            goto CLOSE;
        }
        
        int bytes = recv(client, str, 256, 0);

        if (bytes > 0)
        {
            str[bytes] = '\0';
            printf("echo> %s", str);
        }
        else
        {
            
            if (bytes < 0)
            {
                perror("recv");
            } 
            else
            {
                printf("Server closed connection\n");
                goto CLOSE;
            } 
        }
    }

CLOSE:
    close(client);

EXIT:
    return ret_val;
}