/*
 *    wldlib.h    --    Header file for the WLD library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Main header file for the WLD library, which contains all the
 *    functions and structures for Terraria's world format.
 */
#pragma once

#include "wld.h"

/*
 *    Creates a new Terraria world.
 *
 *    @param int width           The width of the world.
 *    @param int height          The height of the world.
 *    @param const char *name    The name of the world.
 *    @param const char *seed    The seed of the world.
 * 
 *    @return wld_t *    The created world, or NULL on failure.
 */
wld_t *wld_new(int width, int height, const char *name, const char *seed);

/*
 *    Loads a terraria world.
 *
 *    @param char *path    The file to load.
 *
 *    @return wld_t *    The loaded world, or NULL on failure.
 */
wld_t *wld_open(const char *path);

/*
 *    Writes a world to a file.
 *
 *    @param wld_t *wld    The world to write.
 *    @param char *path      The file to write to.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_write(wld_t *wld, const char *path);

/*
 *    Frees a world.
 *
 *    @param wld_t *wld    The world to free.
 */
void wld_free(wld_t *wld);