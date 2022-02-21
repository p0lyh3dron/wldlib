/*
 *    parseutil.c    --    Source file for the parsing utility functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *
 *    Source file for the parsing utility functions.
 */
#include "parseutil.h"
#include "wldheaderfuncs.h"
#include "wldfuncs.h"

#include <stdio.h>
#include <malloc.h>
/*
 *    Reads a file into a buffer.
 *
 *    @param const s8 *
 *        The file to read.
 *
 *    @return filestream_t *
 *        The file stream.
 */
filestream_t *filestream_open( const s8 *spPath ) {
    filestream_t *spStream = ( filestream_t * )malloc( sizeof( filestream_t ) );
    if( !spStream ) {
        fprintf( stderr, "filestream_open( const u8 * ): Failed to allocate memory for stream.\n" );
        return NULL;
    }

    FILE *fp = fopen( spPath, "rb" );
    if( !fp ) {
        fprintf( stderr, "filestream_open( const u8 * ): Failed to open file.\n" );
        free( spStream );
        return NULL;
    }

    fseek( fp, 0, SEEK_END );
    spStream->aSize = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    spStream->apBuf = ( u8 * )malloc( spStream->aSize );
    if( !spStream->apBuf ) {
        fprintf( stderr, "filestream_open( const u8 * ): Failed to allocate memory for buffer.\n" );
        free( spStream );
        return NULL;
    }

    fread( spStream->apBuf, 1, spStream->aSize, fp );
    fclose( fp );
    spStream->aPos = 0;

    return spStream;
}
/*
 *    Frees a file stream.
 *
 *    @param filestream_t *
 *        The file stream to free.
 */
void filestream_free( filestream_t *spStream ) {
    if( !spStream ) {
        fprintf( stderr, "filestream_free( filestream_t * ): Stream is NULL.\n" );
        return;
    }

    if( !spStream->apBuf ) {
        fprintf( stderr, "filestream_free( filestream_t * ): Buffer is NULL.\n" );
        return;
    }

    free( spStream->apBuf );
    free( spStream );
}
/*
 *    Parses a string.
 *
 *    @param u8 *
 *       The buffer to parse.
 *    @param u32
 *       The position to start parsing at.
 *
 *    @return u8 *
 *       The parsed string, NULL on failure.
 */
s8 *parse_string( u8 *spBuf, u32 *spPos ) {
    u8 len = 0;

    PARSE( spBuf, *spPos, u8, len );
    if( len == 0 ) {
        return NULL;
    }

    s8 *str = ( s8 * )malloc( len + 1 );
    if( !str ) {
        fprintf( stderr, "parse_string( u8 *, u32 ): Failed to allocate memory for string.\n" );
        return NULL;
    }

    PARSE_ARRAY( spBuf, *spPos, s8, str, len );
    str[ len ] = '\0';

    return str;
}
/*
 *    Determines the the file version and
 *    calls the appropriate function to parse.
 * 
 *    @param wld_t *
 *        The world to parse.
 */
void wld_decude_parsing_type( wld_t *spWld ) {
    s32 version = wld_get_version( spWld );

    if ( version == -1 ) {
        fprintf( stderr, "wld_decude_parsing_type( wld_t* ): World is NULL.\n" );
        return;
    }

    spWld->aVer = version;
    
    switch ( version ) {
        case 244:
            wld_header_parse( spWld );
            break;
        default:
            fprintf( stderr, "wld_decude_parsing_type( s32 ): Unknown version.\n" );
            break;
    }
}