/*
 *    filestream.h    --    File stream
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Header file for the file stream
 *    (which is used for reading and writing files)
 */
#pragma once

#include "types.h"

typedef struct {
    unsigned char *buf;
    unsigned int   len;
    unsigned int   pos;
} filestream_t;