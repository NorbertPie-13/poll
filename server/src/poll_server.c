#include "poll_server.h"

int poll_server_add(struct pollfd **poll_structs, int client_fd, int* poll_count, int * max_size)
{
    int ret_val = EXIT_FAILURE;
    struct pollfd *new_poll = NULL;
    if ((NULL == poll_structs) || (NULL == *poll_structs) || (NULL == poll_count) || (NULL == max_size))
    {
        fprintf(stderr, "%s recieved NULL pointer.\n", __func__);
        goto EXIT;
    }
    if (*poll_count == *max_size)
    {
        *max_size *= DOUBLE;
        new_poll = calloc(*max_size, sizeof(*new_poll));
        if (NULL == new_poll)
        {
            fprintf(stderr, "Failed allocation in %s", __func__);
            goto EXIT;
        }
        new_poll = (struct pollfd*)memmove(new_poll, *poll_structs, (sizeof(**poll_structs) * (*poll_count)));
        if (new_poll == NULL)
        {
            perror("Memmove");
            // *poll_structs = new_poll;
            goto EXIT;
        }
        free(*poll_structs);
        *poll_structs = new_poll;
    }
    (*poll_structs)[*poll_count].fd = client_fd;
    (*poll_structs)[*poll_count].events = POLLIN;
    *poll_count += NEW_STRUCT;
    ret_val = EXIT_SUCCESS;
    EXIT:
        return ret_val;
}

int poll_remove(struct pollfd *poll_structs, int index, int * poll_count)
{
    int ret_val = EXIT_FAILURE;
    if ((NULL == poll_structs) || (NULL == poll_count))
    {
        fprintf(stderr, "%s recieved NULL pointer.\n", __func__);
        goto EXIT;
    }
    if ((0 > index) || (index > *poll_count))
    {
        fprintf(stderr, "Invalid index : %s\n", __func__);
        goto EXIT;
    }

    poll_structs[index] = poll_structs[*poll_count - 1];
    (*poll_count)--;
    ret_val = EXIT_SUCCESS;
    EXIT:
        return ret_val;
}

/* End of File */
