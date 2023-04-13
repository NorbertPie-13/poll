#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Exit codes for main.
 * 
 */
enum exit_states
{
    PASS,
    FAIL
};

/**
 * @brief Packed structure for file length and name length.
 * @size 8 bytes
 * 
 */
typedef struct __attribute__ ((__packed__))
{
    int32_t file_name_length;
    int32_t file_length;
} file_header_t;

/**
 * @brief A return code for the client.
 * @size 1 byte
 */
typedef struct
{
    uint8_t return_code;
} response_t;

#endif