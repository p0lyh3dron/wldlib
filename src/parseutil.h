/*
 *    parseutil.h    --    parse utility functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 * 
 *    Declares functions for parsing strings/files.
 */
#pragma once

#define PARSE( buf, pos, type, var )             \
    var = *( type* )( buf + pos );               \
    pos += sizeof( type )

#define PARSE_ARRAY( buf, pos, type, var, size ) \
    for( u32 i = 0; i < size; i++ ) {            \
        var[ i ] = *( type* )( buf + pos );      \
        pos += sizeof( type );                   \
    }

#include "filestream.h"
#include "wld.h"
/*
 *    Reads a file into a buffer.
 *
 *    @param const s8 *
 *        The file to read.
 *
 *    @return filestream_t *
 *        The file stream.
 */
filestream_t *filestream_open( const s8 *spPath );
/*
 *    Frees a file stream.
 *
 *    @param filestream_t *
 *        The file stream to free.
 */
void filestream_free( filestream_t *spStream );
/*
 *    Parses a string.
 *
 *    @param u8 *
 *       The buffer to parse.
 *    @param u32 *
 *       The position to start parsing at.
 *
 *    @return s8 *
 *       The parsed string, NULL on failure.
 */
s8 *parse_string( u8 *spBuf, u32 *spPos );
/*
 *    Determines the the file version and
 *    calls the appropriate function to parse.
 * 
 *    @param wld_t *
 *        The world to parse.
 */
void wld_decude_parsing_type( wld_t *spWld );