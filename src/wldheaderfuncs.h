/*
 *    wldheaderfuncs.h    --    header file for the wld header functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
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
 *    @param wld_t*
 *        The world to check.
 *
 *    @return s32
 *        The version of the world.
 */
s32 wld_get_version(wld_t *spWld);
/*
 *    Parses the world info header.
 *
 *    @param wld_t*
 *        The world to parse.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_info_parse(wld_t *spWld);
/*
 *    Parses the world format header.
 *
 *    @param wld_t*
 *        The world to parse.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_header_parse(wld_t *spWld);
/*
 *    Returns the world info header as a buffer.
 *
 *    @param wld_t *
 *        The world to get the header from.
 *    @param u32 *
 *        The length of the header.
 *
 *    @return s8 *
 *        The world info header.
 */
s8 *wld_info_get_header(wld_t *spWld, u32 *spLen);
/*
 *    Returns the world format header as a buffer.
 *
 *    @param wld_t *
 *        The world to get the header from.
 *    @param u32 *
 *        The length of the header.
 *
 *    @return s8 *
 *        The world format header.
 */
s8 *wld_header_get_header(wld_t *spWld, u32 *spLen);
/*
 *    Dumps the contents of the world info header to stdout.
 *
 *    @param wld_info_header_t
 *        The header to dump.
 */
void wld_info_header_dump(wld_info_header_t sInfo);
/*
 *    Dumps the contents of the world format header to stdout.
 *
 *    @param wld_header_t
 *        The header to dump.
 */
void wld_header_dump(wld_header_t sHeader);
/*
 *    Frees a world info header.
 *
 *    @param wld_info_header_t
 *        The world header to free.
 */
void wld_info_header_free(wld_info_header_t sHeader);
/*
 *    Frees a world format header.
 *
 *    @param wld_header_t
 *        The world header to free.
 */
void wld_header_free(wld_header_t sHeader);