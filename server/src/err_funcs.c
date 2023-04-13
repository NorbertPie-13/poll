
/**
 * @file error_funcs.c
 * @author Matthew N. Pieczynski (matthew.n.pieczynski.mil@army.mil)
 * @brief A module of error checking functions to be used in multiple programs.
 * @version 0.1
 * @date 2023-01-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "err_funcs.h"

typedef void (*signal_handler)(int);
/**
 * @brief Check pointer for reference.
 *
 * @param pointer Pointer to be checked against
 * @param msg Function name to be passed.
 * @return Return 1 on failure and 0 on success
 */
void err_funcs_print_error(const char * p_name)
{
    if (NULL == p_name)
    {
        return;
    }
    fprintf(stderr, "%s recieved NULL pointer.\n", p_name);
}


off_t
err_get_size(char *p_file)
{
    off_t ret_val = ERROR;
    if (NULL == p_file)
    {
        fprintf(stderr, "%s recieved NULL pointer.\n", __func__);
        goto EXIT;
    }

    errno                 = 0;
    struct stat file_info = { 0 };
    if (-1 == stat(p_file, &file_info))
    {
        perror("stat");
        errno = 0;
        goto EXIT;
    }

    ret_val = file_info.st_size;
EXIT:
    return ret_val;
}

int err_funcs_check_num_string(char * p_string)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == p_string)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    size_t string_sz = strlen(p_string);
    for (size_t letter_index = 0; letter_index < string_sz; ++letter_index)
    {
        if (NINE < p_string[letter_index])
        {
            goto EXIT_INVALID_FORMAT;
        }
        if (BACKSLASH > p_string[letter_index])
        {
            
            goto EXIT_INVALID_FORMAT;
        }
    }

    ret_val = EXIT_SUCCESS;
    goto EXIT;

    EXIT_INVALID_FORMAT:
        fprintf(stderr, "Port must be numerical value.\n");

    EXIT:
        return ret_val;
}


/**
 * @brief Check for function to if socket function is in Error state
 *
 * @param exp Socket file descriptor
 * @param msg Message to be displayed in error state.
 * @return returns the file descriptor.
 */
int
err_funcs_chk(int exp, char *p_msg)
{
    int ret_val = exp;
    // check pointer for message
    if (NULL == p_msg)
    {
        err_funcs_print_error(__func__);
        goto EXIT;
    }
    // check socket for -1 as indicating error
    if (ERROR == exp)
    {
        perror(p_msg);
    }
    // return socket
EXIT:
    return ret_val;
}

/* End of File */
