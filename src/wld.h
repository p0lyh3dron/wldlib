/*
 *    wld.h    --    Header file for the WLD structure
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Main header file for the WLD structure, which contains all the
 *    structures for parsing Terraria's world file contents.
 */
#pragma once

#include "filestream.h"
#include "tile.h"
#include "wldheader.h"

typedef struct {
    filestream_t *file;

    unsigned int      ver;
    wld_info_header_t info;
    wld_header_t      header;
    tile_t          **tiles;
} wld_t;