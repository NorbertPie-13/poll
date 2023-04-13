#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

enum exit_states
{
    PASS,
    FAIL
};

typedef struct __attribute__ ((__packed__))
{
    int32_t file_name_length;
    int32_t file_length;
} file_header_t;

typedef struct
{
    uint8_t return_code;
} response_t;

#endif