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


#include "client_main.h"

#define MAX_LEN 107
#define ERROR -1


int main(int argc, char ** argv)
{
    int ret_val = EXIT_FAILURE;
    if (3 != argc)
    {
        fprintf(stderr, "./<executable> <UNIX_file> <file to get size of>\n");
        goto EXIT;
    }

    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
    };
    
    int client = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == client)
    {
        perror("socket");
        goto EXIT;
    }

    printf("Trying to connect...\n");

    snprintf(remote.sun_path, sizeof(remote.sun_path), "%s", argv[1]);
    socklen_t len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (-1 == connect(client, (struct sockaddr *)&remote, len))
    {
        perror("connect");
        goto CLOSE;
    }

    printf("Connected.\n");

    file_header_t header = { 0 };
    header.file_name_length = strnlen(argv[2], 1024);
    header.file_length = err_get_size(argv[2]);
    if (0 == header.file_length)
    {
        goto EXIT;
    }


    if (-1 == send(client, &header, sizeof(file_header_t), 0))
    {
        perror("send");
        goto CLOSE;
    }
    response_t response = { 0 };
    
    int bytes = recv(client, &response, 1, 0);
    if (-1 == bytes)
    {
        perror("Recv");
        errno = 0;
        goto CLOSE;
    }
    if (0 == response.return_code)
    {
        printf("Success.\n");
    }
    else
    {
        printf("Failed.\n");
    }

    
CLOSE:
    close(client);

EXIT:
    return ret_val;
}
