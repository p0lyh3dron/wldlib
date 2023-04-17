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
#include <math.h>

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
            /*if (fabs(200*tan(pow(x-2100,2)/200000.)+600 - y) < 100) {
                wld->tiles[x][y].tile = 327;
            }*/
        }
    }


    wld_write(wld, "world_modified.wld");

    wld_free(wld);

    return 0;
}