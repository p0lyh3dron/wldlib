/*
 *    parseutil.c    --    Source file for the parsing utility functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Source file for the parsing utility functions.
 */
#include "parseutil.h"
#include "log.h"
#include "wldfuncs.h"
#include "wldheaderfuncs.h"

#include <malloc.h>
#include <stdio.h>

/*
 *    Reads a file into a buffer.
 *
 *    @param const char *path    The file to read.
 *
 *    @return filestream_t *   The file stream.
 */
filestream_t *filestream_open(const char *path) {
    filestream_t *stream = (filestream_t *)malloc(sizeof(filestream_t));

    if (stream == (filestream_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for stream.\n");
        return (filestream_t *)0x0;
    }

    FILE *fp = fopen(path, "rb");

    if (fp == (FILE *)0x0) {
        LOGF_ERR("Failed to open file.\n");
        free(stream);
        return (filestream_t *)0x0;
    }

    fseek(fp, 0, SEEK_END);
    stream->len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    stream->buf = (unsigned char *)malloc(stream->len);

    if (stream->buf == (unsigned char *)0x0) {
        LOGF_ERR("Failed to allocate memory for buffer.\n");
        free(stream);
        return (filestream_t *)0x0;
    }

    fread(stream->buf, 1, stream->len, fp);
    fclose(fp);
    stream->pos = 0;

    return stream;
}

/*
 *    Seeks into a file stream.
 *
 *    @param filestream_t *stream    The file stream to seek into.
 *    @param int                     The position to seek to.
 */
void filestream_seek(filestream_t *stream, int pos) {
    stream->pos = pos;
}

/*
 *    Frees a file stream.
 *
 *    @param filestream_t *stream    The file stream to free.
 */
void filestream_free(filestream_t *stream) {
    if (stream == (filestream_t *)0x0) {
        LOGF_ERR("Stream is NULL.\n");
        return;
    }

    if (stream->buf == (unsigned char *)0x0) {
        LOGF_ERR("Buffer is NULL.\n");
        return;
    }

    free(stream->buf);
    free(stream);
}

/*
 *    Parses a string.
 *
 *    @param unsigned char *buf    The buffer to parse.
 *    @param unsigned int pos    The position to start parsing at.
 *
 *    @return unsigned char *      The parsed string, NULL on failure.
 */
char *parse_string(unsigned char *buf, unsigned int *pos) {
    unsigned char len = 0;

    PARSE(buf, *pos, unsigned char, len);
    if (len == 0) {
        return (char *)0x0;
    }

    char *str = (char *)malloc(len + 1);
    if (str == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for string.\n");
        return (char *)0x0;
    }

    PARSE_ARRAY(buf, *pos, char, str, len);
    str[len] = '\0';

    return str;
}

/*
 *    Pushes a new byte into a buffer.
 *
 *    @param char **    The buffer to push into.
 *    @param unsigned char       The byte to push.
 *    @param unsigned int      The size of the buffer.
 *
 *    @return unsigned int     The new size of the buffer, 0 on failure.
 */
unsigned int push_byte(char **buf, unsigned char byte, unsigned int len) {
    if (len >= 0xFFFFFFFF) {
        LOGF_FAT("Buffer size is too large.\n");
        return 0;
    }
    if (buf == (char **)0x0 || *buf == (char *)0x0) {
        LOGF_FAT("Buffer is NULL.\n");
        return 0;
    }

    VLOGF_MSG("Pushing byte %d into buffer of size %d.\n", byte, len);

    *buf = (char *)realloc(*buf, len + 1);
    if (*buf == (char *)0x0) {
        LOGF_FAT("Failed to reallocate buffer.\n");
        return 0;
    }

    (*buf)[len + 1] = byte;
    return len + 1;
}

/*
 *    Determines the the file version and
 *    calls the appropriate function to parse.
 *
 *    @param wld_t *wld    The world to parse.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_decude_parsing_type(wld_t *wld) {
    int version = wld_get_version(wld);

    if (version == -1) {
        LOGF_ERR("World is NULL.\n");
        return 0;
    }

    wld->ver = version;

    switch (version) {
    case 244:
        wld_header_parse(wld);
        return 1;
        break;
    case 245:
        wld_header_parse(wld);
        return 1;
        break;
    case 246:
        wld_header_parse(wld);
        return 1;
        break;
    case 279:
        wld_header_parse(wld);
        return 1;
        break;
    default:
        VLOGF_FAT("Unknown version: %d\n", version);
        return 0;
    }
}