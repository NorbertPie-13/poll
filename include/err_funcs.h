/**
 * @file error_funcs.h
 * @author Matthew N. Pieczynski (matthew.n.pieczynski.mil@army.mil)
 * @brief A module of error checking functions to be used in multiple programs.
 * @version 0.1
 * @date 2023-01-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ERR_FUNCS_H
#define ERR_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#define ERROR    -1
#define FAIL     1
#define NINE 58
#define BACKSLASH 47

/**
 * @brief Check pointer for reference.
 *
 * @param name Function name to be passed.
 * @return void
 */
void err_funcs_print_error(const char * p_name);

/**
 * @brief Check for function to if socket function is in Error state
 *
 * @param exp Socket file descriptor
 * @param msg Message to be displayed in error state.
 * @return returns the file descriptor.
 */
int err_funcs_chk(int exp, char *p_msg);

/**
 * @brief Check a string to ensure all values are numerical.
 * 
 * @return Return 1 on error(or not all numerical values), or 0 on success. 
 */
int err_funcs_check_num_string(char * p_string);

#endif