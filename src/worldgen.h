/*
 *    worldgen.h    --    Header file for world generation functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on April 17, 2023
 *
 *    Function declarations for world generation can be found here.
 */
#ifndef WLD_WORLDGEN_H
#define WLD_WORLDGEN_H

#include "wld.h"

typedef struct {
    unsigned char crimson_left;
    int           copper;
    int           copper_bar;
    int           iron;
    int           iron_bar;
    int           silver;
    int           silver_bar;
    int           gold;
    int           gold_bar;
} genvars_t;

/*
 *    Generates a world.
 *
 *    @param wld_t        *wld      The world to generate.
 *    @param unsigned int  seed     The seed to use for generation.
 *    @param char         *name     The name of the world.
 *    @param int           width    The width of the world.
 *    @param int           height   The height of the world.
 * 
 *    @return int    0 on success, -1 on failure.
 */
int wld_gen_world(wld_t *wld, unsigned int seed, char *name, int width, int height);

#endif /* WLD_WORLDGEN_H  */