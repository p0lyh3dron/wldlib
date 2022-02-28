/*
 *    tilefuncs.c    --    Source file for the tile functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Source file for the tile functions.
 */
#include "tilefuncs.h"
#include "parseutil.h"
#include "log.h"

#include <stdio.h>
#include <malloc.h>
#include <spng.h>
/*
 *    Compares two tiles.
 *
 *    @param tile_t
 *        The first tile to compare.
 *    @param tile_t
 *        The second tile to compare.
 *
 *    @return u32
 *        1 if the tiles are equal, 0 if they are not.
 */
u32 tile_compare( tile_t sTile0, tile_t sTile1 ) {
    if ( sTile0.aTile != sTile1.aTile ) {
        return 0;
    }
    if ( sTile0.aU != sTile1.aU ) {
        return 0;
    }
    if ( sTile0.aV != sTile1.aV ) {
        return 0;
    }
    if ( sTile0.aWall != sTile1.aWall ) {
        return 0;
    }
    if ( sTile0.aLiquidType != sTile1.aLiquidType ) {
        return 0;
    }
    if ( sTile0.aLiquidAmount != sTile1.aLiquidAmount ) {
        return 0;
    }
    if ( sTile0.aWireFlags != sTile1.aWireFlags ) {
        return 0;
    }
    if ( sTile0.aOrientation != sTile1.aOrientation ) {
        return 0;
    }
    if ( sTile0.aPaintedTile != sTile1.aPaintedTile ) {
        return 0;
    }
    if ( sTile0.aPaintedWall != sTile1.aPaintedWall ) {
        return 0;
    }
    return 1;
}
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
    /* Seek to the tile data.  */
    filestream_seek( spWld->apFile, spWld->aInfo.apSections[ 1 ] );

    spWld->apTiles = ( tile_t ** )malloc( sizeof( tile_t * ) * spWld->aHeader.aWidth );
    if ( !spWld->apTiles ) {
        fprintf( stderr, "get_tiles( wld_t* ): failed to allocate memory for tiles\n" );
        return;
    }

    for ( s32 x = 0; x < spWld->aHeader.aWidth; ++x ) {

        spWld->apTiles[ x ] = ( tile_t * )malloc( sizeof( tile_t ) * spWld->aHeader.aHeight );
        if ( !spWld->apTiles[ x ] ) {
            fprintf( stderr, "get_tiles( wld_t* ): failed to allocate memory for tile column %d\n", x );
            return;
        }

        for ( s32 y = 0; y < spWld->aHeader.aHeight; ) {
            tile_t t = { 0 };
            t.aTile  = -1;
            t.aWall  = -1;
                
            u8  activeFlags    = 0;
            u8  tileFlagsLow   = 0;
            u8  tileFlagsHigh  = 0;
            s16 tempWall       = 0;
            s16 copies         = 0;
            /* Read the first byte.  */
            PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, activeFlags );
            /* If the first bit is set, read the next byte.  */
            if ( activeFlags & 1 << 0 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tileFlagsLow );
                /* If the second byte's first bit is set, read the next byte.  */
                if ( tileFlagsLow & 1 << 0 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tileFlagsHigh );
                }
            }
            /* Bit 1: Tile is present.  */
            if ( activeFlags & 1 << 1 ) {
                /* Bit 5: Tile is 16 bits.  */
                if ( activeFlags & 1 << 5 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u16, t.aTile );
                }
                else {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aTile );
                }
                /* If tile is important (lookup in info header), read texture UVs.  */
                if ( tile_is_important( spWld, t ) ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, t.aU );
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, t.aV );
                }
                /* High tile flags bit 3: Tile is painted.  */
                if ( tileFlagsHigh & 1 << 3 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aPaintedTile );
                }
            }
            /* Bit 2: Wall is present.  */
            if ( activeFlags & 1 << 2 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aWall );
                /* High tile flags bit 4: Wall is painted.  */
                if ( tileFlagsHigh & 1 << 4 ) {
                    PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aPaintedWall );
                }
            }
            /* Bits 3-4: Liquid is present, next byte is the liquid amount.  */
            if ( activeFlags & ( 1 << 3 | 1 << 4 ) ) {
                t.aLiquidType = ( activeFlags & ( 1 << 3 | 1 << 4 ) ) >> 3;
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, t.aLiquidAmount );
            }
            /* Low tile flags bit 1: Red wire present.  */
            if ( tileFlagsLow & 1 << 1 ) {
                t.aWireFlags |= WIRE_RED;
            }
            /* Low tile flags bit 2: Blue wire present.  */
            if ( tileFlagsLow & 1 << 2 ) {
                t.aWireFlags |= WIRE_BLUE;
            }
            /* Low tile flags bit 3: Green wire present.  */
            if ( tileFlagsLow & 1 << 3 ) {
                t.aWireFlags |= WIRE_GREEN;
            }
            /* Low tile flags bits 4-6: Tile orientation.  */
            if ( tileFlagsLow & ( 1 << 4 | 1 << 5 | 1 << 6 ) ) {
                t.aOrientation = ( tileFlagsLow & ( 1 << 4 | 1 << 5 | 1 << 6 ) ) >> 4;
            }
            /* High tile flags bit 1: Actuator present.  */
            if ( tileFlagsHigh & 1 << 1 ) {
                t.aWireFlags |= WIRE_ACTUATOR;
            }
            /* High tile flags bit 2: Actuator active.  */
            if ( tileFlagsHigh & 1 << 2 ) {
                t.aWireFlags |= WIRE_ACTIVE_ACTUATOR;
            }
            /* High tile flags bit 5: Yellow wire present.  */
            if ( tileFlagsHigh & 1 << 5 ) {
                t.aWireFlags |= WIRE_YELLOW;
            }
            /* High tile flags bit 6: Next byte is the 8 bit extension of wall id.  */
            if ( tileFlagsHigh & 1 << 6 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, tempWall );
                t.aWall |= tempWall << 8;
            }
            /* Bits 6-7 = 1: [1,255] copies of the tile.  */
            if ( ( activeFlags & ( 1 << 6 | 1 << 7 ) ) >> 6 == 1 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, u8, copies );
            }
            /* Bits 6-7 = 2: [1,65535] copies of the tile.  */
            else if ( ( activeFlags & ( 1 << 6 | 1 << 7 ) ) >> 6 == 2 ) {
                PARSE( spWld->apFile->apBuf, spWld->apFile->aPos, s16, copies );
            }
            /* Uncompress RLE.  */
            for ( s32 i = 0; i <= copies && y < spWld->aHeader.aHeight; ++i, ++y ) {
                spWld->apTiles[ x ][ y ] = t;
            }
        }

    }
}
/*
 *    Returns the tile as a buffer.
 *
 *    @param wld_t *
 *        The world to get the tile from.
 *    @param u32 *
 *        The length of the buffer.
 * 
 *    @return s8 *
 *        The tile as a buffer.
 */

#define TILE_WRITE_TILE_FLAGS_LOW  ( 1 << 0 )
#define TILE_WRITE_TILE_FLAGS_HIGH ( 1 << 1 )
#define TILE_WRITE_TILE_ID         ( 1 << 2 )
#define TILE_WRITE_TILE_ID16       ( 1 << 3 )
#define TILE_WRITE_TILE_UV         ( 1 << 4 )
#define TILE_WRITE_TILE_COLOR      ( 1 << 5 )
#define TILE_WRITE_WALL_ID         ( 1 << 6 )
#define TILE_WRITE_WALL_ID16       ( 1 << 7 )
#define TILE_WRITE_WALL_COLOR      ( 1 << 8 )
#define TILE_WRITE_LIQUID_AMT      ( 1 << 9 )
#define TILE_WRITE_COPIES          ( 1 << 10 )
#define TILE_WRITE_COPIES16        ( 1 << 11 )

s8 *tile_get_buffer( wld_t *spWld, u32 *spLen ) {
    if ( !spWld ) {
        fprintf( stderr, "tile_get_buffer( wld_t*, u32* ): world is NULL\n" );
        return NULL;
    }
    if ( !spLen ) {
        fprintf( stderr, "tile_get_buffer( wld_t*, u32* ): length is NULL\n" );
        return NULL;
    }
    
    s8 *pBuf = ( s8 * )malloc( spWld->aHeader.aWidth * spWld->aHeader.aHeight * 17 * sizeof( s8 ) );
    if ( !pBuf ) {
        fprintf( stderr, "tile_get_buffer( wld_t*, u32* ): failed to allocate memory for buffer\n" );
        return NULL;
    }
    u32 len = 0;
    for ( s32 x = 0; x < spWld->aHeader.aWidth; ++x ) {
        for ( s32 y = 0; y < spWld->aHeader.aHeight; ++y ) {
            tile_t *pTile = &spWld->apTiles[ x ][ y ];
            u8  activeFlags    = 0;
            u8  tileFlagsLow   = 0;
            u8  tileFlagsHigh  = 0;
            u32 writeFlags     = 0;

            /* Tile is present.  */
            if ( pTile->aTile != -1 ) {
                activeFlags |= 1 << 1;
                /* Tile is 16 bits.  */
                if ( pTile->aTile & 0xFF00 ) {
                    activeFlags |= 1 << 5;
                    writeFlags  |= TILE_WRITE_TILE_ID16;
                }
                else {
                    writeFlags  |= TILE_WRITE_TILE_ID;
                }
                /* If tile is important (lookup in info header), write texture UVs.  */
                if ( tile_is_important( spWld, *pTile ) ) {
                    writeFlags |= TILE_WRITE_TILE_UV;
                }
                /* Tile is painted.  */
                if ( pTile->aPaintedTile ) {
                    tileFlagsHigh |= 1 << 3;
                    writeFlags    |= TILE_WRITE_TILE_COLOR;
                }
            }
            /* Wall is present.  */
            if ( pTile->aWall != -1 ) {
                activeFlags |= 1 << 2;
                writeFlags        |= TILE_WRITE_WALL_ID;
                /* Wall is 16 bits.  */
                if ( pTile->aWall & 0xFF00 ) {
                    tileFlagsHigh |= 1 << 6;
                    writeFlags    |= TILE_WRITE_WALL_ID16;
                }
                if ( pTile->aPaintedWall ) {
                    tileFlagsHigh |= 1 << 4;
                    writeFlags    |= TILE_WRITE_WALL_COLOR;
                }
            }
            if ( pTile->aOrientation ) {
                tileFlagsLow |= pTile->aOrientation << 4;
            }
            /* Liquid is present.  */
            if ( pTile->aLiquidAmount ) {
                activeFlags |= pTile->aLiquidType << 3;
                writeFlags  |= TILE_WRITE_LIQUID_AMT;
            }
            if ( pTile->aWireFlags & WIRE_RED ) {
                tileFlagsLow |= 1 << 1;
            }
            if ( pTile->aWireFlags & WIRE_GREEN ) {
                tileFlagsLow |= 1 << 3;
            }
            if ( pTile->aWireFlags & WIRE_BLUE ) {
                tileFlagsLow |= 1 << 2;
            }
            if ( pTile->aWireFlags & WIRE_YELLOW ) {
                tileFlagsHigh |= 1 << 5;
            }
            if ( pTile->aWireFlags & WIRE_ACTUATOR ) {
                tileFlagsHigh |= 1 << 1;
            }
            if ( pTile->aWireFlags & WIRE_ACTIVE_ACTUATOR ) {
                tileFlagsHigh |= 1 << 2;
            }
            if ( tileFlagsHigh ) {
                tileFlagsLow |= 1 << 0;
                writeFlags   |= TILE_WRITE_TILE_FLAGS_HIGH;
            }
            if ( tileFlagsLow ) {
                activeFlags |= 1 << 0;
                writeFlags  |= TILE_WRITE_TILE_FLAGS_LOW;
            }
            /* Tile is copied.  */
            u32 copies = 0;
            for ( s32 i = 1; y < spWld->aHeader.aHeight; ++i ) {
                if ( tile_compare( spWld->apTiles[ x ][ y + i ], *pTile ) ) {
                    ++copies;
                }
                else {
                    y += i - 1;
                    break;
                }
            }
            if ( copies ) {
                if ( copies > 0xFF ) {
                    activeFlags |= 1 << 7;
                    writeFlags  |= TILE_WRITE_COPIES16;
                }
                else {
                    activeFlags |= 1 << 6;
                    writeFlags  |= TILE_WRITE_COPIES;
                }
            }
            /* Write flags.  */
                /*if ( !push_byte( &pBuf, activeFlags, len++ ) ) {
                    fprintf( stderr, "tile_get_buffer( wld_t*, u32* ): failed to push active flags\n" );
                    free( pBuf );
                    return NULL;
                }*/
                pBuf[ len++ ] = activeFlags;
            /*if ( pBuf[ len - 1 ] != ( s8 )*( spWld->apFile->apBuf + spWld->aInfo.apSections[ 1 ] +len - 1 ) ) {
                log_error( "tile_get_buffer( wld_t*, u32* ): active flags don't match at position %d\n", len - 1 );
            }*/
            u32 ret = 0;
            ret += append_u8( &pBuf, tileFlagsLow, &len, writeFlags, TILE_WRITE_TILE_FLAGS_LOW );
            ret += append_u8( &pBuf, tileFlagsHigh, &len, writeFlags, TILE_WRITE_TILE_FLAGS_HIGH );
            ret += append_u8( &pBuf, pTile->aTile, &len, writeFlags, TILE_WRITE_TILE_ID );
            ret += append_u16( &pBuf, pTile->aTile, &len, writeFlags, TILE_WRITE_TILE_ID16 );
            ret += append_u16( &pBuf, pTile->aU, &len, writeFlags, TILE_WRITE_TILE_UV );
            ret += append_u16( &pBuf, pTile->aV, &len, writeFlags, TILE_WRITE_TILE_UV );
            ret += append_u8( &pBuf, pTile->aPaintedTile, &len, writeFlags, TILE_WRITE_TILE_COLOR );
            ret += append_u8( &pBuf, pTile->aWall & 0xFF, &len, writeFlags, TILE_WRITE_WALL_ID );
            ret += append_u8( &pBuf, pTile->aPaintedWall, &len, writeFlags, TILE_WRITE_WALL_COLOR );
            ret += append_u8( &pBuf, pTile->aLiquidAmount, &len, writeFlags, TILE_WRITE_LIQUID_AMT );
            ret += append_u8( &pBuf, ( u8 )( ( pTile->aWall & 0xFF00 ) >> 8 ), &len, writeFlags, TILE_WRITE_WALL_ID16 );
            ret += append_u8( &pBuf, copies, &len, writeFlags, TILE_WRITE_COPIES );
            ret += append_u16( &pBuf, copies, &len, writeFlags, TILE_WRITE_COPIES16 );

            if ( ret != 13 ) {
                fprintf( stderr, "tile_get_buffer( wld_t*, u32* ): failed to write tile\n" );
                free( pBuf );
                return NULL;
            }
        }
    }
    *spLen = len;

    return pBuf;
}
/*
 *    Appends a u8 to the buffer.
 *
 *    @param s8 **
 *        The buffer to append to.
 *    @param u8
 *        The flag to append.
 *    @param u32
 *        The length of the buffer.
 *    @param u32
 *        The write flags.
 *    @param u32
 *        The flag to check.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 append_u8( s8 **spBuf, u8 sFlag, u32 *spLen, u32 sFlags, u32 sCheck ) {
    if ( !spBuf ) {
        fprintf( stderr, "append_u8( s8*, u8, u32, u32, u32 ): buffer is NULL\n" );
        return 0;
    }
    if ( !spLen ) {
        fprintf( stderr, "append_u8( s8*, u8, u32, u32, u32 ): length is NULL\n" );
        return 0;
    }
    if ( !sCheck ) {
        fprintf( stderr, "append_u8( s8*, u8, u32, u32, u32 ): no check flag!\n" );
        return 0;
    }
    if ( sFlags & sCheck ) {
        /*if ( !push_byte( spBuf, sFlag, *spLen ) ) {
            fprintf( stderr, "append_u8( s8*, u8, u32, u32, u32 ): failed to push flag\n" );
            return 0;
        }*/
        (*spBuf)[ *spLen ] = sFlag;
        ++*spLen;
    }
    return 1;
}
/*
 *    Appends a u16 to the buffer.
 *
 *    @param s8 **
 *        The buffer to append to.
 *    @param u16
 *        The flag to append.
 *    @param u32
 *        The length of the buffer.
 *    @param u32
 *        The write flags.
 *    @param u32
 *        The flag to check.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 append_u16( s8 **spBuf, u16 sFlag, u32 *spLen, u32 sFlags, u32 sCheck ) {
    if ( !spBuf ) {
        fprintf( stderr, "append_u16( s8*, u16, u32, u32, u32 ): buffer is NULL\n" );
        return 0;
    }
    if ( !spLen ) {
        fprintf( stderr, "append_u16( s8*, u16, u32, u32, u32 ): length is NULL\n" );
        return 0;
    }
    if ( !sCheck ) {
        fprintf( stderr, "append_u16( s8*, u16, u32, u32, u32 ): no check flag!\n" );
        return 0;
    }
    if ( sFlags & sCheck ) {
        /*if ( !push_byte( spBuf, sFlag & 0xFF, *spLen ) ) {
            fprintf( stderr, "append_u16( s8*, u16, u32, u32, u32 ): failed to push low byte\n" );
            return 0;
        }
        ++*spLen;
        if ( !push_byte( spBuf, ( sFlag >> 8 ) & 0xFF, *spLen ) ) {
            fprintf( stderr, "append_u16( s8*, u16, u32, u32, u32 ): failed to push high byte\n" );
            return 0;
        }  */
        (*spBuf)[ *spLen ] = sFlag & 0xFF;
        (*spBuf)[ *spLen + 1 ] = ( sFlag >> 8 ) & 0xFF;
        *spLen += 2;
    }
    return 1;
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
/*
 *    Dumps the tiles to an image.
 *
 *    @param wld_t *
 *        The world to dump the tiles from.
 *    @param char *
 *        The path to the image to dump to.
 * 
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 dump_tiles( wld_t *spWld, char *spPath ) {
    if ( !spWld ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): world is NULL\n" );
        return 0;
    }

    if ( !spPath ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): path is NULL\n" );
        return 0;
    }

    size_t len = sizeof( u8 ) * spWld->aHeader.aWidth * spWld->aHeader.aHeight * 4;

    u8 *pBuf = ( u8 * )malloc( len );
    if ( !pBuf ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): failed to allocate memory for image\n" );
        return 0;
    }

    for ( int x = 0; x < spWld->aHeader.aWidth; ++x ) {

        for ( int y = 0; y < spWld->aHeader.aHeight; ++y ) {
            pBuf[ ( y * spWld->aHeader.aWidth + x ) * 4 + 0 ] = spWld->apTiles[ x ][ y ].aTile;
            pBuf[ ( y * spWld->aHeader.aWidth + x ) * 4 + 1 ] = spWld->apTiles[ x ][ y ].aTile;
            pBuf[ ( y * spWld->aHeader.aWidth + x ) * 4 + 2 ] = spWld->apTiles[ x ][ y ].aTile;
            pBuf[ ( y * spWld->aHeader.aWidth + x ) * 4 + 3 ] = 255;
        }
    }

    spng_ctx *pCtx = spng_ctx_new( SPNG_CTX_ENCODER );

    spng_set_option( pCtx, SPNG_ENCODE_TO_BUFFER, 1 );

    struct spng_ihdr ihdr = { 0 };

    ihdr.width  = spWld->aHeader.aWidth;
    ihdr.height = spWld->aHeader.aHeight;
    ihdr.bit_depth = 8;
    ihdr.color_type = 6;

    spng_set_ihdr( pCtx, &ihdr );

    int ret;
    if ( ( ret = spng_encode_image( pCtx, pBuf, len, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE ) ) ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): spng_encode_image() failed: %s\n", spng_strerror( ret ) );
        free( pBuf );
        return 0;
    }

    FILE *pFile = fopen( spPath, "wb" );
    if ( !pFile ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): failed to open file for writing: %s\n", spPath );
        free( pBuf );
        return 0;
    }

    void *pData = spng_get_png_buffer( pCtx, &len, &ret );

    if ( !pData ) {
        fprintf( stderr, "dump_tiles( wld_t*, char* ): spng_get_png_buffer() failed: %s\n", spng_strerror( ret ) );
        free( pBuf );
        return 0;
    }

    fwrite( pData, 1, len, pFile );

    fclose( pFile );

    free( pData );
    free( pBuf );

    return 1;
}