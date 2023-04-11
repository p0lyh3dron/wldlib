/*
 *    wld.h    --    Header file for the WLD structure
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Main header file for the WLD structure, which contains all the
 *    structures for parsing Terraria's world file contents.
 */
#pragma once

#include "filestream.h"
#include "tile.h"
#include "wldheader.h"

typedef struct {
    filestream_t *apFile;

    u32               aVer;
    wld_info_header_t aInfo;
    wld_header_t      aHeader;
    tile_t          **apTiles;
} wld_t;