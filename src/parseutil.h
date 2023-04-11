/*
 *    parseutil.h    --    parse utility functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Declares functions for parsing strings/files.
 */
#pragma once

#define PARSE(buf, pos, type, var) \
    var = *(type *)(buf + pos);    \
    pos += sizeof(type)

#define PARSE_ARRAY(buf, pos, type, var, size) \
    for (int i = 0; i < size; i++) {           \
        var[i] = *(type *)(buf + pos);         \
        pos += sizeof(type);                   \
    }

#include "filestream.h"
#include "wld.h"

/*
 *    Reads a file into a buffer.
 *
 *    @param const char *path    The file to read.
 *
 *    @return filestream_t *   The file stream.
 */
filestream_t *filestream_open(const char *path);

/*
 *    Seeks into a file stream.
 *
 *    @param filestream_t *stream    The file stream to seek into.
 *    @param int                     The position to seek to.
 */
void filestream_seek(filestream_t *stream, int pos);

/*
 *    Frees a file stream.
 *
 *    @param filestream_t *stream    The file stream to free.
 */
void filestream_free(filestream_t *stream);

/*
 *    Parses a string.
 *
 *    @param unsigned char *buf    The buffer to parse.
 *    @param unsigned int pos    The position to start parsing at.
 *
 *    @return unsigned char *      The parsed string, NULL on failure.
 */
char *parse_string(unsigned char *buf, unsigned int *pos);

/*
 *    Pushes a new byte into a buffer.
 *
 *    @param char **    The buffer to push into.
 *    @param unsigned char       The byte to push.
 *    @param unsigned int      The size of the buffer.
 *
 *    @return unsigned int     The new size of the buffer, 0 on failure.
 */
unsigned int push_byte(char **buf, unsigned char byte, unsigned int len);

/*
 *    Determines the the file version and
 *    calls the appropriate function to parse.
 *
 *    @param wld_t *wld    The world to parse.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_decude_parsing_type(wld_t *wld);