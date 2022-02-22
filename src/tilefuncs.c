/*
 *    tilefuncs.c    --    Source file for the tile functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Source file for the tile functions.
 */
#include "tilefuncs.h"
#include "parseutil.h"

#include <stdio.h>
#include <malloc.h>
/*
 *    Returns whether or not a tile is important.
 *
 *    @param wld_t *
 *       The world to get the tiles from.
 *    @param tile_t
 *        The tile to check.
 * 
 *    @return u32
 *        1 if the tile is important, 0 if not.
 */
u32 tile_is_important( wld_t *spWld, tile_t sTile ) {
    if ( spWld->aInfo.apUVs[ sTile.aTile / 8 ] & ( 1 << ( sTile.aTile % 8 ) ) ) {
        return 1;
    }
    return 0;
}
/*
 *    Returns the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to get the tiles from.
 */
void get_tiles( wld_t *spWld ) {
    if ( !spWld ) {
        fprintf( stderr, "get_tiles( wld_t* ): world is NULL\n" );
    }

    filestream_seek( spWld->apFile, spWld->aInfo.apSections[ 1 ] );

    spWld->apTiles = ( tile_t ** )malloc( sizeof( tile_t * ) * spWld->aHeader.aWidth );
    for ( s32 x = 0; x < spWld->aHeader.aWidth; ++x ) {

        spWld->apTiles[ x ] = ( tile_t * )malloc( sizeof( tile_t ) * spWld->aHeader.aHeight );
        for ( s32 y = 0; y < spWld->aHeader.aHeight; ++y ) {
            tile_t t = { 0 };
                
            u8  activeFlags    = 0;
            u8  tileFlagsLow   = 0;
            u8  tileFlagsHigh  = 0;
            s16 tempWall;
            s16 copies;
            PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, activeFlags );

            if ( activeFlags & 1 << 0 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tileFlagsLow );
                if ( tileFlagsLow & 1 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tileFlagsHigh );
                }
            }
            if ( activeFlags & 1 << 1 ) {
                if ( activeFlags & 1 << 5 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, t.aTile );
                }
                else {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s8, t.aTile );
                }
                if ( tile_is_important( spWld, t ) ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, t.aU );
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, t.aV );
                }
                if ( tileFlagsHigh & 1 << 3 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aPaintedTile );
                }
            }
            if ( activeFlags & 1 << 2 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aWall );
                if ( tileFlagsHigh & 1 << 4 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aPaintedWall );
                }
            }
            if ( activeFlags & ( 1 << 3 | 1 << 4 ) ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aLiquidAmount );
            }
            if ( tileFlagsHigh & 1 << 6 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tempWall );
                t.aWall |= tempWall << 8;
            }
            if ( ( activeFlags & ( 1 << 6 | 1 << 7 ) ) >> 6 == 1 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, copies );
            }
            else if ( ( activeFlags & ( 1 << 6 | 1 << 7 ) ) >> 6 == 2 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, copies );
            }
            for ( s32 i = 0; i < copies; ++i, ++y ) {
                spWld->apTiles[ x ][ y ] = t;
            }
        }

    }
}
/*
 *    Frees the list of tiles in the world.
 *
 *    @param wld_t *
 *        The world to free the tiles from.
 */
void free_tiles( wld_t *spWld ) {
    if ( !spWld ) {
        fprintf( stderr, "free_tiles( wld_t* ): world is NULL\n" );
    }

    for ( s32 x = 0; x < spWld->aHeader.aWidth; ++x ) {
        free( spWld->apTiles[ x ] );
    }
    free( spWld->apTiles );
}