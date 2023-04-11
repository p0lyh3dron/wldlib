/*
 *    tilefuncs.h    --    header file for tile functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Declares the tile functions.
 */
#pragma once

#include "tile.h"
#include "wld.h"
/*
 *    Compares two tiles.
 *
 *    @param tile_t t0    The first tile to compare.
 *    @param tile_t t1    The second tile to compare.
 *
 *    @return unsigned int         1 if the tiles are equal, 0 if they are not.
 */
unsigned int tile_compare(tile_t t0, tile_t t1);

/*
 *    Returns whether or not a tile is important.
 *
 *    @param wld_t  *wld     The world to get the tiles from.
 *    @param tile_t  tile    The tile to check.
 *
 *    @return unsigned int            1 if the tile is important, 0 if not.
 */
unsigned int tile_is_important(wld_t *wld, tile_t tile);

/*
 *    Returns the list of tiles in the world.
 *
 *    @param wld_t *wld    The world to get the tiles from.
 */
void get_tiles(wld_t *wld);

/*
 *    Returns the tile as a buffer.
 *
 *    @param wld_t *wld     The world to get the tile from.
 *    @param unsigned int   *size    The length of the buffer.
 *
 *    @return char *    The tile as a buffer.
 */
char *tile_get_buffer(wld_t *wld, unsigned int *size);

/*
 *    Appends a unsigned char to the buffer.
 *
 *    @param char  **buf     The buffer to append to.
 *    @param unsigned char    flag    The flag to append.
 *    @param unsigned int  *len     The length of the buffer.
 *    @param unsigned int   flags   The write flags.
 *    @param unsigned int   check   The flag to check.
 *    @param unsigned int  *ret     The return value.
 */
void append_u8(char **buf, unsigned char flag, unsigned int *len, unsigned int flags, unsigned int check, unsigned int *ret);

/*
 *    Appends a unsigned short to the buffer.
 *
 *    @param char  **buf     The buffer to append to.
 *    @param unsigned short   flag    The flag to append.
 *    @param unsigned int  *len     The length of the buffer.
 *    @param unsigned int   flags   The write flags.
 *    @param unsigned int   check   The flag to check.
 *    @param unsigned int  *ret     The return value.
 */
void append_u16(char **buf, unsigned short flag, unsigned int *len, unsigned int flags, unsigned int check, unsigned int *ret);

/*
 *    Frees the list of tiles in the world.
 *
 *    @param wld_t *wld    The world to free the tiles from.
 */
void free_tiles(wld_t *wld);

/*
 *    Dumps the tiles to a png.
 *
 *    @param wld_t *wld    The world to dump the tiles from.
 *    @param char *path    The path to the image to dump to.
 *
 *    @return unsigned int    1 on success, 0 on failure.
 */
unsigned int dump_tiles_png(wld_t *wld, char *path);