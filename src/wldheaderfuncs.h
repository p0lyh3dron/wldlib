/*
 *    wldheaderfuncs.h    --    header file for the wld header functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Declares the functions that will be used to parse the world header.
 *    The functions are defined in the wldheaderfuncs.c file.
 */
#pragma once

#include "wld.h"

/*
 *    Peeks at the world header and returns the version of the world.
 *    Returns -1 if the world is invalid.
 *
 *    @param wld_t *wld    The world to check.
 *
 *    @return int          The version of the world.
 */
int wld_get_version(wld_t *wld);

/*
 *    Parses the world info header.
 *
 *    @param wld_t* wld    The world to parse.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_info_parse(wld_t *wld);

/*
 *    Parses the world format header.
 *
 *    @param wld_t* wld    The world to parse.
 *
 *    @return unsigned int    1 on success, 0 on failure.
 */
unsigned int wld_header_parse(wld_t *wld);

/*
 *    Returns the world info header as a buffer.
 *
 *    @param wld_t *wld      The world to get the header from.
 *    @param unsigned int   *len      The length of the header.
 *
 *    @return char *           The world info header.
 */
char *wld_info_get_header(wld_t *wld, unsigned int *len);

/*
 *    Returns the world format header as a buffer.
 *
 *    @param wld_t *wld    The world to get the header from.
 *    @param unsigned int   *len    The length of the header.
 *
 *    @return char *         The world format header.
 */
char *wld_header_get_header(wld_t *wld, unsigned int *len);

/*
 *    Dumps the contents of the world format header to stdout.
 *
 *    @param wld_header_t header    The header to dump.
 */
void wld_header_dump(wld_header_t header);

/*
 *    Dumps the contents of the world info header to stdout.
 *
 *    @param wld_info_header_t info    The header to dump.
 */
void wld_info_header_dump(wld_info_header_t info);

/*
 *    Frees a world info header.
 *
 *    @param wld_info_header_t header    The world header to free.
 */
void wld_info_header_free(wld_info_header_t header);
/*
 *    Frees a world format header.
 *
 *    @param wld_header_t    header    The world header to free.
 */
void wld_header_free(wld_header_t header);