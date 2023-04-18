/*
 *    worldgen.c    --    Source file for world generation functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on April 17, 2023
 *
 *    Function definitions for world generation can be found here.
 */
#include "worldgen.h"

#include "log.h"
#include "rand.h"

genvars_t _genvars;

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
int wld_gen_world(wld_t *wld, unsigned int seed, char *name, int width, int height) {
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("World is NULL!");
        return -1;
    }

    if (seed == (char *)0x0) {
        LOGF_ERR("Seed is NULL!");
        return -1;
    }

    if (name == (char *)0x0) {
        LOGF_ERR("Name is NULL!");
        return -1;
    }
    
    if (seed == 5162020) {
        wld->header.drunk = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "not the bees") == 0 || strcmp(wld->header.seed, "not the bees!") == 0) {
        wld->header.bees = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "notraps") == 0 || strcmp(wld->header.seed, "no traps") == 0) {
        wld->header.no_traps = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "for the worthy") == 0) {
        wld->header.ftw = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "celebrationmk10") == 0 || seed == 5162011 || seed == 5162021) {
        wld->header.tenth = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "constant") == 0 || 
        strcmp(wld->header.seed, "theconstant") == 0 ||
        strcmp(wld->header.seed, "the constant") == 0 ||
        strcmp(wld->header.seed, "eye4aneye") == 0 ||
        strcmp(wld->header.seed, "eyeforaneye") == 0) {
        wld->header.dont_starve = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "don't dig up") == 0 ||
        strcmp(wld->header.seed, "dont dig up") == 0 ||
        strcmp(wld->header.seed, "dontdigup") == 0) {
        wld->header.remix = 1;
        seed = next_max(999999999);
    }

    if (strcmp(wld->header.seed, "get fixed boi") == 0 ||
        strcmp(wld->header.seed, "getfixedboi") == 0) {
        wld->header.zenith = 1;
        wld->header.remix = 1;
        wld->header.dont_starve = 1;
        wld->header.tenth = 1;
        wld->header.ftw = 1;
        wld->header.no_traps = 1;
        wld->header.bees = 1;
        wld->header.drunk = 1;
        seed = next_max(999999999);
    }

    wld->header.spawn_x = width / 2;
    wld->header.spawn_y = height / 2;

    set_seed(seed);

    LOGF_NOTE("Generation: Resetting world...");
    if (next_max(2) == 0)
        _genvars.crimson_left = 0;
    else
        _genvars.crimson_left = 1;

    wld->header.num_clouds = next_minmax(10, 200);
    wld->header.wind_speed = 0.f;
    
    while (wld->header.wind_speed == 0.f)
        wld->header.wind_speed = (float)next_minmax(-400, 401) * 0.001f;

    int hell_items[5]      = {274, 220, 112, 218, 3019};
    int hell_items_rand[5] = {0, 0, 0, 0, 0};
    /* Put the unholy trident in place of fire flower.  */
    if (wld->header.remix == 1) 
        hell_items[2] = 683;

    int count = 5;
    while (--count > 0) {
        int index = next_max(count);
        hell_items_rand[4 - count] = hell_items[index];

        unsigned int i;
        for (i = index; i < count; i++)
            hell_items[i] = hell_items[i + 1];
    }

    wld->header.slime_rain_time = -next_minmax(86400 * 2, 86400 * 3);
    wld->header.cloud_bg        = -next_minmax(8640, 86400);

    if (next_max(2) == 0) {
        _genvars.copper       = 166;
        _genvars.copper_bar   = 703;
        wld->header.copper_id = 166;
    } else {
        _genvars.copper       = 7;
        _genvars.copper_bar   = 20;
        wld->header.copper_id = 7;
    }

    if ((!wld->header.dont_starve || wld->header.drunk) && next_max(2) == 0) {
        _genvars.iron       = 167;
        _genvars.iron_bar   = 704;
        wld->header.iron_id = 167;
    } else {
        _genvars.iron       = 6;
        _genvars.iron_bar   = 22;
        wld->header.iron_id = 6;
    }

    if (next_max(2) == 0) {
        _genvars.silver       = 168;
        _genvars.silver_bar   = 705;
        wld->header.silver_id = 168;
    } else {
        _genvars.silver       = 9;
        _genvars.silver_bar   = 21;
        wld->header.silver_id = 9;
    }

    if ((!wld->header.dont_starve || wld->header.drunk) && next_max(2) == 0) {
        _genvars.gold       = 169;
        _genvars.gold_bar   = 706;
        wld->header.gold_id = 169;
    } else {
        _genvars.gold       = 8;
        _genvars.gold_bar   = 19;
        wld->header.gold_id = 8;
    }

    wld->header.crimson = next_max(2) == 0;
    wld->header.id      = next_max(2147483647);

    if (width <= 4200) {
        wld->header.tree_x[0] = next_minmax(width * .25, width * .75);
        wld->header.tree_styles[0] = next_max(6);
        wld->header.tree_styles[1] = next_max(6);

        while (wld->header.tree_styles[1] == wld->header.tree_styles[0])
            wld->header.tree_styles[1] = next_max(6);

        wld->header.tree_x[1] = width;
        wld->header.tree_x[2] = width;

        unsigned long i;
        for (i = 0; i < 2; i++) {
            if (wld->header.tree_styles[i] == 0 && next_max(3) != 0) {
                wld->header.tree_styles[i] = 4;
            }
        }
    }
    /* ADD DIFF WORLD SIZES LATER  */
    if (width <= 4200) {
        wld->header.cave_back_x[0] = next_minmax(width * .25, width * .75);
        wld->header.cave_back_x[1] = width;
        wld->header.cave_back_x[2] = width;
        wld->header.cave_back_style[0] = next_max(8);
        wld->header.cave_back_style[1] = next_max(8);
        
        while (wld->header.cave_back_style[1] == wld->header.cave_back_style[0])
            wld->header.cave_back_style[1] = next_max(8);
    }
    /* PICK UP RaandomizeBackGrounds  */
}