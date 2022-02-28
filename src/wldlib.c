/*
 *    wldlib.c    --    Source file for the WLD library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 * 
 *    Source file for the WLD library, which contains all the
 *    functions definitions for parsing Terraria's world format.
 */
#include "wldlib.h"
#include "parseutil.h"
#include "wldheaderfuncs.h"
#include "tilefuncs.h"
#include "log.h"

#include <stdio.h>
#include <malloc.h>
/*
 *    Loads a terraria world.
 *
 *    @param s8 *
 *        The file to load.
 * 
 *    @return wld_t *
 *        The loaded world, or NULL on failure.
 */
wld_t *wld_open( const s8 *spPath ) {
    wld_t *pWld = ( wld_t * )malloc( sizeof( wld_t ) );
    if( !pWld ) {
        fprintf( stderr, "wld_open( const u8 * ): Failed to allocate memory for world.\n" );
        return NULL;
    }

    filestream_t *pStream = filestream_open( spPath );
    if( !pStream ) {
        fprintf( stderr, "wld_open( const u8 * ): Failed to open file.\n" );
        free( pWld );
        return NULL;
    }

    pWld->apFile = pStream;

    if ( !wld_decude_parsing_type( pWld ) ) {
        log_fatal( "wld_open( const u8 * ): Failed to decode parsing type.\n" );
        return NULL;
    }

    get_tiles( pWld );
    dump_tiles( pWld, "tiles.png" );

    return pWld;
}
/*
 *    Writes a world to a file.
 *
 *    @param wld_t *
 *        The world to write.
 *    @param s8 *
 *        The file to write to.
 *
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_write( wld_t *spWld, const s8 *spPath ) {
    FILE *pFile = fopen( spPath, "wb" );
    if( !pFile ) {
        fprintf( stderr, "wld_write( wld_t *, const u8 * ): Failed to open file.\n" );
        return 0;
    }

    fwrite( spWld->apFile->apBuf, spWld->aInfo.apSections[ 1 ], 1, pFile );

    u32 tileLen = 0;
    s8 *pTile   = tile_get_buffer( spWld, &tileLen );
    fwrite( pTile, tileLen, 1, pFile );
    fwrite( spWld->apFile->apBuf + spWld->aInfo.apSections[ 1 ] + tileLen, spWld->apFile->aSize - tileLen - spWld->aInfo.apSections[ 1 ], 1, pFile );
    free( pTile );

    fclose( pFile );

    return 1;
}
/*
 *    Frees a world.
 *
 *    @param wld_t *
 *        The world to free.
 */
void wld_free( wld_t *spWld ) {
    if( !spWld ) {
        fprintf( stderr, "wld_free( wld_t * ): World is NULL.\n" );
        return;
    }
    if( spWld->apFile ) {
        filestream_free( spWld->apFile );
    }
    free_tiles( spWld );
    wld_header_free( spWld->aHeader );
    wld_info_header_free( spWld->aInfo );
    free( spWld );
}