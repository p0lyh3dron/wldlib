/*
 *    example.c    --    example of using the wldlib library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    An example of using the wldlib library.
 */
#include "wldlib.h"

#include <stdio.h>

/*
 *    Entry.
 *
 *    @return int
 *        0 on success, -1 on failure.
 */
int main(void) {
    wld_t *wld = wld_open("world.wld");

    if (wld == (wld_t *)0x0) {
        printf("Failed to open world.wld\n");
        return -1;
    }

    int x;
    int y;
    for (x = 0; x < wld->header.width; ++x) {
        for (y = 0; y < wld->header.height; ++y) {
            if (wld->tiles[x][y].tile == 0)
                wld->tiles[x][y].tile = 54;
            if (wld->tiles[x][y].tile == 2) {
                wld->tiles[x][y].tile = -1;
                wld->tiles[x][y].liquid_type   = LIQUID_LAVA;
                wld->tiles[x][y].liquid_amount = 255;
            }
            if (wld->tiles[x][y].liquid_type == LIQUID_WATER)
                wld->tiles[x][y].liquid_type = LIQUID_LAVA;
        }
    }

    wld_write(wld, "world_modified.wld");

    wld_free(wld);

    return 0;
}