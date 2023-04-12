/*
 *    wldlib.c    --    Source file for the WLD library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Source file for the WLD library, which contains all the
 *    functions definitions for parsing Terraria's world format.
 */
#include "wldlib.h"
#include "log.h"
#include "parseutil.h"
#include "tilefuncs.h"
#include "wldheaderfuncs.h"

#include <malloc.h>
#include <stdio.h>

/*
 *    Loads a terraria world.
 *
 *    @param char *path    The file to load.
 *
 *    @return wld_t *    The loaded world, or NULL on failure.
 */
wld_t *wld_open(const char *path) {
    wld_t *wld = (wld_t *)malloc(sizeof(wld_t));
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for world.\n");
        return (wld_t *)0x0;
    }

    filestream_t *pStream = filestream_open(path);

    if (pStream == (filestream_t *)0x0) {
        LOGF_ERR("Failed to open file.\n");
        free(wld);
        return (wld_t *)0x0;
    }

    wld->file = pStream;

    if (wld_decude_parsing_type(wld) == 0) {
        LOGF_FAT("Failed to decode parsing type.\n");
        return (wld_t *)0x0;
    }

    get_tiles(wld);
    dump_tiles_png(wld, "tiles.png");

    return wld;
}

/*
 *    Writes a world to a file.
 *
 *    @param wld_t *wld    The world to write.
 *    @param char *path      The file to write to.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_write(wld_t *wld, const char *path) {
    FILE *pFile = fopen(path, "wb");

    if (pFile == (FILE *)0x0) {
        LOGF_ERR("Failed to open file.\n");
        return 0;
    }

    unsigned int temp;

    int tileLen = 0;
    char        *tiles   = tile_get_buffer(wld, &tileLen);
    long         diff    = wld->info.sections[2] - wld->info.sections[1] - tileLen;

    unsigned long i;
    for (i = 2; i < wld->info.numsections; i++) {
        wld->info.sections[i] -= diff;
    }

    fwrite(wld_info_get_header(wld, &temp), wld->info.sections[0], 1, pFile);
    fwrite(wld->file->buf + wld->info.sections[0], wld->info.sections[1] - wld->info.sections[0], 1, pFile);

    if (tileLen != wld->info.sections[2] - wld->info.sections[1]) {
        LOGF_ERR("Tile length mismatch, This might fuck things up.\n");
        VLOGF_NOTE("Tile length: %u, expected: %u, diff: %d\n", tileLen, wld->info.sections[2] - wld->info.sections[1],
                                                                wld->info.sections[2] - wld->info.sections[1] - tileLen);
        return 0;
    }

    fwrite(tiles, tileLen, 1, pFile);
    fwrite(wld->file->buf + wld->info.sections[1] + tileLen + diff, wld->file->len - tileLen - diff - wld->info.sections[1], 1, pFile);
    free(tiles);

    fclose(pFile);

    return 1;
}

/*
 *    Frees a world.
 *
 *    @param wld_t *wld    The world to free.
 */
void wld_free(wld_t *wld) {
    if (wld == (wld_t *)0x0) {
        LOGF_WARN("World is NULL.\n");
        return;
    }

    if (wld->file != (filestream_t *)0x0)
        filestream_free(wld->file);

    free_tiles(wld);
    wld_header_free(wld->header);
    wld_info_header_free(wld->info);
    free(wld);
}