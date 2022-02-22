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
u32 tile_is_important( wld_t *spWld, tile_t sTile );
/*
 *    Returns the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to get the tiles from.
 */
void get_tiles( wld_t *spWld );
/*
 *    Frees the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to free the tiles from.
 */
void free_tiles( wld_t *spWld );