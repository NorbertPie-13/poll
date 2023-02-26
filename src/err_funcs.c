
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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include "err_funcs.h"

#define MAX_ARGS 4
#define ERROR    -1
#define FAIL     1
#define NINE 58
#define BACKSLASH 47
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

/**
 * @brief Validate the number of arguments for the program.
 *
 * @param args The amount of arguments taken from commandline
 * @return Return 1 on failure, 0 on success.
 */
int
err_funcs_validate_argc(int args)
{
    int ret_val = EXIT_FAILURE;
    // check if args are not equal to 4 (number of arguments needed for program)
    if (MAX_ARGS != args)
    {
        // if not display format for program to run
        fprintf(stderr,
                "./atm <listen_port> <credentials_file> <balance_file>\n");
        goto EXIT;
    }
    // exit gracefully
    ret_val = EXIT_SUCCESS;
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

/** TODO: REMOVE AS PROJECT SPECIFIC*/
// int
// err_funcs_check_cli(int *p_args, char **pp_argv)
// {
//     int ret_val = EXIT_FAILURE;
    
//     // check the pointers
//     if ((NULL == p_args) || (NULL == pp_argv))
//     {
//         err_funcs_print_error(__func__);
//         goto EXIT;
//     }
//     if (MAX_ARGS != *p_args)
//     {
//         fprintf(stderr, "./<atm program path> <port num> <credentials_file.txt> <balance_file.txt>\n");
//         goto EXIT;
//     }
//     // validate the amount of arguments to ensure the correct amount
//     if (NULL == *pp_argv)
//     {
//         goto EXIT;
//     }
//     if (EXIT_FAILURE == check_numerical_string(pp_argv[1]))
//     {
//         goto EXIT;
//     }

//     // validate file size, permission, and names.
//     if ((FAIL == file_io_validate_file(pp_argv[2]))
//         || (FAIL == file_io_validate_file(pp_argv[3])))
//     {
//         goto EXIT;
//     }
//     ret_val = EXIT_SUCCESS;
// EXIT:
//     return ret_val;
// }

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