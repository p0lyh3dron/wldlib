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

    wld_decude_parsing_type( pWld );

    return pWld;
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
    wld_info_header_free( spWld->aInfo );
    free( spWld );
}