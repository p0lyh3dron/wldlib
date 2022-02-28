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
#include "log.h"

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
 *    Seeks into a file stream.
 *
 *    @param filestream_t *
 *        The file stream to seek into.
 *
 *    @param s32
 *        The position to seek to.
 */
void filestream_seek( filestream_t *spStream, s32 aPos ) {
    spStream->aPos = aPos;
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
 *    Pushes a new byte into a buffer.
 *
 *    @param s8 **
 *        The buffer to push into.
 *    @param u8
 *        The byte to push.
 *    @param u32
 *        The size of the buffer.
 *
 *    @return u32
 *        The new size of the buffer, 0 on failure.
 */
u32 push_byte( s8 **spBuf, u8 aByte, u32 aSize ) {   
    if( aSize >= 0xFFFFFFFF ) {
        log_fatal( "push_byte( s8 *, u8, u32 ): Buffer size is too large.\n" );
        return 0;
    }
    if ( spBuf == NULL || *spBuf == NULL ) {
        log_fatal( "push_byte( s8 *, u8, u32 ): Buffer is NULL.\n" );
        return 0;
    }

    log_debug( "push_byte( u8 *, u8, u32 ): Pushing byte %d into buffer of size %d.\n", aByte, aSize );

    *spBuf = ( s8 * )realloc( *spBuf, aSize + 1 );
    if( !*spBuf ) {
        log_fatal( "push_byte( s8 *, u8, u32 ): Failed to reallocate buffer.\n" );
        return 0;
    }

    ( *spBuf )[ aSize + 1 ] = aByte;
    return aSize + 1;
}
/*
 *    Determines the the file version and
 *    calls the appropriate function to parse.
 * 
 *    @param wld_t *
 *        The world to parse.
 * 
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_decude_parsing_type( wld_t *spWld ) {
    s32 version = wld_get_version( spWld );

    if ( version == -1 ) {
        fprintf( stderr, "wld_decude_parsing_type( wld_t* ): World is NULL.\n" );
        return 0;
    }

    spWld->aVer = version;
    
    switch ( version ) {
        case 244:
            wld_header_parse( spWld );
            return 1;
            break;
        case 245:
            wld_header_parse( spWld );
            return 1;
            break;
        case 246:
            wld_header_parse( spWld );
            return 1;
            break;
        default:
            log_fatal( "wld_decude_parsing_type( s32 ): Unknown version: %d\n", version );
            return 0;
    }
}