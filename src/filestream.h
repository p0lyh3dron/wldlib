/*
 *    filestream.h    --    File stream
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Header file for the file stream
 *    (which is used for reading and writing files)
 */
#pragma once

#include "types.h"

typedef struct {
    u8 *apBuf;
    u32 aSize;
    u32 aPos;
}filestream_t;