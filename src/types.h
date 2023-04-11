/*
 *    types.h    --    Header file for the WLD types
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Header file the types used in the WLD file.
 */
#pragma once

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef char  s8;
typedef short s16;
typedef int   s32;
typedef long  s64;

typedef float  f32;
typedef double f64;

typedef struct {
    s32 x0;
    s32 x;
    s32 y0;
    s32 y;
} rect_t;