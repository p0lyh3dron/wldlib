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
#include "types.h"

typedef struct {
    filestream_t *file;

    unsigned int      ver;
    wld_info_header_t info;
    wld_header_t      header;
    tile_t          **tiles;
    short             chest_count;
    chest_t          *chests;
    short             sign_count;
    sign_t           *signs;
    unsigned long     npc_count;
    unsigned long     other_count;
    npc_t            *npcs;
    int               tile_entity_count;
    tile_entity_t    *tile_entities;
    int               pressure_plate_count;
    pressure_plate_t *pressure_plates;
    int               town_element_count;
    town_element_t   *town_elements;
    int               kill_count;
    kill_t           *kills;
    int               tracker_count;
    tracker_t        *trackers;
    int               chatter_count;
    chatted_t        *chatters;
    unsigned long     creative_powers_len;
    char             *creative_powers;
} wld_t;