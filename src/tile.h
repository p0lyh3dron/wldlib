/*
 *    tile.h    --    header file for the tile structure
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
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
    LIQUID_WATER = 1,
    LIQUID_LAVA  = 2,
    LIQUID_HONEY = 3,
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
    s16 aTile;
    s16 aU;
    s16 aV;
    s16 aWall;
    u8  aLiquidType;
    u8  aLiquidAmount;
    u8  aWireFlags;
    u8  aOrientation;
    u8  aPaintedTile;
    u8  aPaintedWall;
} tile_t;

static const u32 gTilePallete[] = {
    0x976b4b,
};