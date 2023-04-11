/*
 *    tilefuncs.h    --    header file for tile functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Declares the tile functions.
 */
#pragma once

#include "tile.h"
#include "wld.h"
/*
 *    Compares two tiles.
 *
 *    @param tile_t
 *        The first tile to compare.
 *    @param tile_t
 *        The second tile to compare.
 *
 *    @return u32
 *        1 if the tiles are equal, 0 if they are not.
 */
u32 tile_compare(tile_t sTile0, tile_t sTile1);
/*
 *    Returns whether or not a tile is important.
 *
 *    @param wld_t *
 *       The world to get the tiles from.
 *    @param tile_t
 *        The tile to check.
 *
 *    @return u32
 *        1 if the tile is important, 0 if not.
 */
u32 tile_is_important(wld_t *spWld, tile_t sTile);
/*
 *    Returns the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to get the tiles from.
 */
void get_tiles(wld_t *spWld);
/*
 *    Appends a u8 to the buffer.
 *
 *    @param s8 **
 *        The buffer to append to.
 *    @param u8
 *        The flag to append.
 *    @param u32 *
 *        The length of the buffer.
 *    @param u32
 *        The write flags.
 *    @param u32
 *        The flag to check.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 append_u8(s8 **spBuf, u8 sFlag, u32 *spLen, u32 sFlags, u32 sCheck);
/*
 *    Appends a u16 to the buffer.
 *
 *    @param s8 **
 *        The buffer to append to.
 *    @param u16
 *        The flag to append.
 *    @param u32 *
 *        The length of the buffer.
 *    @param u32
 *        The write flags.
 *    @param u32
 *        The flag to check.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 append_u16(s8 **spBuf, u16 sFlag, u32 *spLen, u32 sFlags, u32 sCheck);
/*
 *    Returns the tile as a buffer.
 *
 *    @param wld_t *
 *        The world to get the tile from.
 *    @param u32 *
 *        The length of the buffer.
 *
 *    @return s8 *
 *        The tile as a buffer.
 */
s8 *tile_get_buffer(wld_t *spWld, u32 *spLen);
/*
 *    Frees the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to free the tiles from.
 */
void free_tiles(wld_t *spWld);
/*
 *    Dumps the tiles to an image.
 *
 *    @param wld_t *
 *        The world to dump the tiles from.
 *    @param char *
 *        The path to the image to dump to.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 dump_tiles(wld_t *spWld, char *spPath);