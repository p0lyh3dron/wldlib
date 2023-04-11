/*
 *    wldlib.h    --    Header file for the WLD library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Main header file for the WLD library, which contains all the
 *    functions and structures for Terraria's world format.
 */
#pragma once

#include "wld.h"
/*
 *    Loads a terraria world.
 *
 *    @param s8 *
 *        The file to load.
 *
 *    @return wld_t *
 *        The loaded world, or NULL on failure.
 */
wld_t *wld_open(const s8 *spPath);
/*
 *    Writes a world to a file.
 *
 *    @param wld_t *
 *        The world to write.
 *    @param s8 *
 *        The file to write to.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_write(wld_t *spWld, const s8 *spPath);
/*
 *    Frees a world.
 *
 *    @param wld_t *
 *        The world to free.
 */
void wld_free(wld_t *spWld);