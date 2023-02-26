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

/**
 * @brief Check pointer for reference.
 *
 * @param name Function name to be passed.
 * @return void
 */
void err_funcs_print_error(const char * p_name);


/**
 * @brief Validate the number of arguments for the program.
 *
 * @param args The amount of arguments taken from commandline
 * @return Return 1 on failure, 0 on success.
 */
int err_funcs_validate_argc(int args);

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