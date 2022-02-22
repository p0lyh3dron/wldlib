/*
 *    tile.h    --    header file for the tile structure
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 * 
 *    Declares the tile structure.
 */
#pragma once

#include "types.h"

typedef enum {
    TILE_ORIENTATION_NONE,
};

typedef struct {
    s16 aTile;
    s16 aU;
    s16 aV;
    s16 aWall;
    u8 aLiquidType;
    u8 aLiquidAmount;
    u8 aRedWire;
    u8 aBlueWire;
    u8 aGreenWire;
    u8 aYellowWire;
    u8 aOrientation;
    u8 aActuator;
    u8 aActiveActuator;
    u8 aPaintedTile;
    u8 aPaintedWall;
}tile_t;