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
    for( s32 i = 0; i < size; i++ ) {            \
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
 *    Seeks into a file stream.
 *
 *    @param filestream_t *
 *        The file stream to seek into.
 *
 *    @param s32
 *        The position to seek to.
 */
void filestream_seek( filestream_t *spStream, s32 aPos );
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
 *    Pushes a new byte into a buffer.
 *
 *    @param u8 **
 *        The buffer to push into.
 *    @param u8
 *        The byte to push.
 *    @param u32
 *        The size of the buffer.
 *
 *    @return u32
 *        The new size of the buffer, 0 on failure.
 */
u32 push_byte( s8 **spBuf, u8 aByte, u32 aSize );
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
u32 wld_decude_parsing_type( wld_t *spWld );