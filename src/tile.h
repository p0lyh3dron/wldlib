/*
 *    tile.h    --    header file for the tile structure
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Declares the tile structure.
 */
#pragma once

#include "types.h"

enum {
    TILE_ORIENTATION_NONE = 0,
    TILE_ORIENTATION_HALF = 1,
};

enum {
    LIQUID_WATER   = 1,
    LIQUID_LAVA    = 2,
    LIQUID_HONEY   = 3,
    LIQUID_SHIMMER = 4,
};

enum {
    WIRE_RED             = 1 << 0,
    WIRE_BLUE            = 1 << 1,
    WIRE_GREEN           = 1 << 2,
    WIRE_YELLOW          = 1 << 3,
    WIRE_ACTUATOR        = 1 << 4,
    WIRE_ACTIVE_ACTUATOR = 1 << 5,
};

typedef struct {
    short         tile;
    short         u;
    short         v;
    short         wall;
    unsigned char liquid_type;
    unsigned char liquid_amount;
    unsigned char wiring;
    unsigned char orientation;
    unsigned char tile_paint;
    unsigned char wall_paint;
} tile_t;

static const unsigned int _tile_palette[] = {
    0x976b4b,
};