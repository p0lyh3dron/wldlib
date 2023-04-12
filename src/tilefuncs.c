/*
 *    tilefuncs.c    --    Source file for the tile functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Source file for the tile functions.
 */
#include "tilefuncs.h"

#define TILE_WRITE_TILE_FLAGS_LOW  (1 << 0)
#define TILE_WRITE_TILE_FLAGS_HIGH (1 << 1)
#define TILE_WRITE_TILE_ID         (1 << 2)
#define TILE_WRITE_TILE_ID16       (1 << 3)
#define TILE_WRITE_TILE_UV         (1 << 4)
#define TILE_WRITE_TILE_COLOR      (1 << 5)
#define TILE_WRITE_WALL_ID         (1 << 6)
#define TILE_WRITE_WALL_ID16       (1 << 7)
#define TILE_WRITE_WALL_COLOR      (1 << 8)
#define TILE_WRITE_LIQUID_AMT      (1 << 9)
#define TILE_WRITE_COPIES          (1 << 10)
#define TILE_WRITE_COPIES16        (1 << 11)

#include "log.h"
#include "parseutil.h"

#include <malloc.h>
#include <spng.h>
#include <stdio.h>

/*
 *    Compares two tiles.
 *
 *    @param tile_t t0    The first tile to compare.
 *    @param tile_t t1    The second tile to compare.
 *
 *    @return unsigned int         1 if the tiles are equal, 0 if they are not.
 */
unsigned int tile_compare(tile_t t0, tile_t t1) {
    if (t0.tile != t1.tile)
        return 0;

    if (t0.u != t1.u)
        return 0;

    if (t0.v != t1.v)
        return 0;

    if (t0.wall != t1.wall)
        return 0;

    if (t0.liquid_type != t1.liquid_type)
        return 0;

    if (t0.liquid_amount != t1.liquid_amount)
        return 0;

    if (t0.wiring != t1.wiring)
        return 0;

    if (t0.orientation != t1.orientation)
        return 0;

    if (t0.tile_paint != t1.tile_paint)
        return 0;

    if (t0.wall_paint != t1.wall_paint)
        return 0;

    return 1;
}

/*
 *    Returns whether or not a tile is important.
 *
 *    @param wld_t  *wld     The world to get the tiles from.
 *    @param tile_t  tile    The tile to check.
 *
 *    @return unsigned int            1 if the tile is important, 0 if not.
 */
unsigned int tile_is_important(wld_t *wld, tile_t tile) {
    if (wld->info.uvs[tile.tile / 8] & (1 << (tile.tile % 8)))
        return 1;

    return 0;
}

/*
 *    Returns the list of tiles in the world.
 *
 *    @param wld_t *wld    The world to get the tiles from.
 */
void get_tiles(wld_t *wld) {
    if (wld == (wld_t *)0x0)
        LOGF_ERR("world is NULL\n");

    /* Seek to the tile data.  */
    filestream_seek(wld->file, wld->info.sections[1]);

    wld->tiles = (tile_t **)malloc(sizeof(tile_t *) * wld->header.width);
    if (wld->tiles == (tile_t **)0x0) {
        LOGF_ERR("failed to allocate memory for tiles\n");
        return;
    }

    int x;
    int y;
    for (x = 0; x < wld->header.width; ++x) {
        wld->tiles[x] = (tile_t *)malloc(sizeof(tile_t) * wld->header.height);
        if (wld->tiles[x] == (tile_t *)0x0) {
            VLOGF_ERR("failed to allocate memory for tile column %d\n", x);
            return;
        }

        for (y = 0; y < wld->header.height;) {
            tile_t t = {0};
            t.tile = -1;
            t.wall = -1;

            unsigned char activeFlags     = 0;
            unsigned char tileFlagsLow    = 0;
            unsigned char tileFlagsHigh   = 0;
            unsigned char additionalFlags = 0;
            short         tempWall        = 0;
            short         copies          = 0;

            /* Read the first byte.  */
            PARSE(wld->file->buf, wld->file->pos, unsigned char, activeFlags);

            /* If the first bit is set, read the next byte.  */
            if (activeFlags & 1 << 0) {
                PARSE(wld->file->buf, wld->file->pos, unsigned char, tileFlagsLow);

                /* If the second byte's first bit is set, read the next byte.  */
                if (tileFlagsLow & 1 << 0) {
                    PARSE(wld->file->buf, wld->file->pos, unsigned char, tileFlagsHigh);

                    /* If the third byte's first bit is set, read the next byte.  */
                    if (tileFlagsHigh & 1 << 0) {
                        PARSE(wld->file->buf, wld->file->pos, unsigned char, additionalFlags);
                    }
                }
            }

            /* Bit 1: Tile is present.  */
            if (activeFlags & 1 << 1) {
                /* Bit 5: Tile is 16 bits.  */
                if (activeFlags & 1 << 5) {
                    PARSE(wld->file->buf, wld->file->pos, unsigned short, t.tile);
                } else {
                    PARSE(wld->file->buf, wld->file->pos, unsigned char, t.tile);
                }

                /* If tile is important (lookup in info header), read texture UVs.  */
                if (tile_is_important(wld, t)) {
                    PARSE(wld->file->buf, wld->file->pos, short, t.u);
                    PARSE(wld->file->buf, wld->file->pos, short, t.v);
                }

                /* High tile flags bit 3: Tile is painted.  */
                if (tileFlagsHigh & 1 << 3) {
                    PARSE(wld->file->buf, wld->file->pos, unsigned char, t.tile_paint);
                }
            }

            /* Bit 2: Wall is present.  */
            if (activeFlags & 1 << 2) {
                PARSE(wld->file->buf, wld->file->pos, unsigned char, t.wall);
                /* High tile flags bit 4: Wall is painted.  */

                if (tileFlagsHigh & 1 << 4) {
                    PARSE(wld->file->buf, wld->file->pos, unsigned char, t.wall_paint);
                }
            }

            /* Bits 3-4: Liquid is present, next byte is the liquid amount.  */
            if (activeFlags & (1 << 3 | 1 << 4)) {
                if (tileFlagsHigh & 1 << 8) 
                    t.liquid_type = LIQUID_SHIMMER;
                else
                    t.liquid_type = (activeFlags & (1 << 3 | 1 << 4)) >> 3;

                PARSE(wld->file->buf, wld->file->pos, unsigned char, t.liquid_amount);
            }

            /* Low tile flags bit 1: Red wire present.  */
            if (tileFlagsLow & 1 << 1) {
                t.wiring |= WIRE_RED;
            }

            /* Low tile flags bit 2: Blue wire present.  */
            if (tileFlagsLow & 1 << 2) {
                t.wiring |= WIRE_BLUE;
            }

            /* Low tile flags bit 3: Green wire present.  */
            if (tileFlagsLow & 1 << 3) {
                t.wiring |= WIRE_GREEN;
            }

            /* Low tile flags bits 4-6: Tile orientation.  */
            if (tileFlagsLow & (1 << 4 | 1 << 5 | 1 << 6)) {
                t.orientation = (tileFlagsLow & (1 << 4 | 1 << 5 | 1 << 6)) >> 4;
            }

            /* High tile flags bit 1: Actuator present.  */
            if (tileFlagsHigh & 1 << 1) {
                t.wiring |= WIRE_ACTUATOR;
            }

            /* High tile flags bit 2: Actuator active.  */
            if (tileFlagsHigh & 1 << 2) {
                t.wiring |= WIRE_ACTIVE_ACTUATOR;
            }

            /* High tile flags bit 5: Yellow wire present.  */
            if (tileFlagsHigh & 1 << 5) {
                t.wiring |= WIRE_YELLOW;
            }

            /* High tile flags bit 6: Next byte is the 8 bit extension of wall id.  */
            if (tileFlagsHigh & 1 << 6) {
                PARSE(wld->file->buf, wld->file->pos, unsigned char, tempWall);
                t.wall |= tempWall << 8;
            }

            /* Bits 6-7 = 1: [1,255] copies of the tile.  */
            if ((activeFlags & (1 << 6 | 1 << 7)) >> 6 == 1) {
                PARSE(wld->file->buf, wld->file->pos, unsigned char, copies);
            }
            /* Bits 6-7 = 2: [1,65535] copies of the tile.  */
            else if ((activeFlags & (1 << 6 | 1 << 7)) >> 6 == 2) {
                PARSE(wld->file->buf, wld->file->pos, short, copies);
            }

            /* Uncompress RLE.  */
            int i;
            for (i = 0; i <= copies && y < wld->header.height; ++i, ++y) {
                wld->tiles[x][y] = t;
            }
        }
    }
}

/*
 *    Returns the tile as a buffer.
 *
 *    @param wld_t *wld     The world to get the tile from.
 *    @param unsigned int   *size    The length of the buffer.
 *
 *    @return char *    The tile as a buffer.
 */
char *tile_get_buffer(wld_t *wld, unsigned int *size) {
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("world is NULL\n");
        return (char *)0x0;
    }
    if (size == (unsigned int *)0x0) {
        LOGF_ERR("length is NULL\n");
        return (char *)0x0;
    }

    char *buf = (char *)malloc(wld->header.width * wld->header.height * 17 * sizeof(char));
    if (buf == (char *)0x0) {
        LOGF_ERR("failed to allocate memory for buffer\n");
        return (char *)0x0;
    }

    unsigned int len = 0;
    int          x;
    int          y;
    for (x = 0; x < wld->header.width; ++x) {
        for (y = 0; y < wld->header.height; ++y) {
            tile_t       *t             = &wld->tiles[x][y];
            unsigned char activeFlags   = 0;
            unsigned char tileFlagsLow  = 0;
            unsigned char tileFlagsHigh = 0;
            unsigned int  writeFlags    = 0;

            /* Tile is present.  */
            if (t->tile != -1) {
                activeFlags |= 1 << 1;

                /* Tile is 16 bits.  */
                if (t->tile & 0xFF00) {
                    activeFlags |= 1 << 5;
                    writeFlags |= TILE_WRITE_TILE_ID16;
                } else {
                    writeFlags |= TILE_WRITE_TILE_ID;
                }

                /* If tile is important (lookup in info header), write texture UVs.  */
                if (tile_is_important(wld, *t))
                    writeFlags |= TILE_WRITE_TILE_UV;

                /* Tile is painted.  */
                if (t->tile_paint) {
                    tileFlagsHigh |= 1 << 3;
                    writeFlags |= TILE_WRITE_TILE_COLOR;
                }
            }

            /* Wall is present.  */
            if (t->wall != -1) {
                activeFlags |= 1 << 2;
                writeFlags |= TILE_WRITE_WALL_ID;

                /* Wall is 16 bits.  */
                if (t->wall & 0xFF00) {
                    tileFlagsHigh |= 1 << 6;
                    writeFlags |= TILE_WRITE_WALL_ID16;
                }

                if (t->wall_paint) {
                    tileFlagsHigh |= 1 << 4;
                    writeFlags |= TILE_WRITE_WALL_COLOR;
                }
            }

            if (t->orientation)
                tileFlagsLow |= t->orientation << 4;

            /* Liquid is present.  */
            if (t->liquid_amount) {
                activeFlags |= t->liquid_type << 3;
                writeFlags |= TILE_WRITE_LIQUID_AMT;

                if (t->liquid_type == LIQUID_SHIMMER)
                    tileFlagsHigh |= 1 << 8;
            }

            if (t->wiring & WIRE_RED)
                tileFlagsLow |= 1 << 1;

            if (t->wiring & WIRE_GREEN)
                tileFlagsLow |= 1 << 3;

            if (t->wiring & WIRE_BLUE)
                tileFlagsLow |= 1 << 2;

            if (t->wiring & WIRE_YELLOW)
                tileFlagsHigh |= 1 << 5;

            if (t->wiring & WIRE_ACTUATOR)
                tileFlagsHigh |= 1 << 1;

            if (t->wiring & WIRE_ACTIVE_ACTUATOR)
                tileFlagsHigh |= 1 << 2;

            if (tileFlagsHigh) {
                tileFlagsLow |= 1 << 0;
                writeFlags |= TILE_WRITE_TILE_FLAGS_HIGH;
            }

            if (tileFlagsLow) {
                activeFlags |= 1 << 0;
                writeFlags |= TILE_WRITE_TILE_FLAGS_LOW;
            }

            /* Tile is copied.  */
            unsigned int copies = 0;
            for (int i = 1; y < wld->header.height; ++i) {
                if (tile_compare(wld->tiles[x][y + i], *t)) {
                    ++copies;
                } else {
                    y += i - 1;
                    break;
                }
            }

            if (copies) {
                if (copies > 0xFF) {
                    activeFlags |= 1 << 7;
                    writeFlags |= TILE_WRITE_COPIES16;
                } else {
                    activeFlags |= 1 << 6;
                    writeFlags |= TILE_WRITE_COPIES;
                }
            }

            unsigned int ret = 0;

            /* Write flags.  */
            buf[len++] = activeFlags;
            append_u8(&buf, tileFlagsLow, &len, writeFlags, TILE_WRITE_TILE_FLAGS_LOW, &ret);
            append_u8(&buf, tileFlagsHigh, &len, writeFlags, TILE_WRITE_TILE_FLAGS_HIGH, &ret);
            append_u8(&buf, t->tile, &len, writeFlags, TILE_WRITE_TILE_ID, &ret);
            append_u16(&buf, t->tile, &len, writeFlags, TILE_WRITE_TILE_ID16, &ret);
            append_u16(&buf, t->u, &len, writeFlags, TILE_WRITE_TILE_UV, &ret);
            append_u16(&buf, t->v, &len, writeFlags, TILE_WRITE_TILE_UV, &ret);
            append_u8(&buf, t->tile_paint, &len, writeFlags, TILE_WRITE_TILE_COLOR, &ret);
            append_u8(&buf, t->wall & 0xFF, &len, writeFlags, TILE_WRITE_WALL_ID, &ret);
            append_u8(&buf, t->wall_paint, &len, writeFlags, TILE_WRITE_WALL_COLOR, &ret);
            append_u8(&buf, t->liquid_amount, &len, writeFlags, TILE_WRITE_LIQUID_AMT, &ret);
            append_u8(&buf, (unsigned char)((t->wall & 0xFF00) >> 8), &len, writeFlags, TILE_WRITE_WALL_ID16, &ret);
            append_u8(&buf, copies, &len, writeFlags, TILE_WRITE_COPIES, &ret);
            append_u16(&buf, copies, &len, writeFlags, TILE_WRITE_COPIES16, &ret);

            if (ret != 0) {
                LOGF_ERR("failed to write tile\n");
                free(buf);
                return (char *)0x0;
            }
        }
    }
    *size = len;

    return buf;
}

/*
 *    Appends a unsigned char to the buffer.
 *
 *    @param char  **buf     The buffer to append to.
 *    @param unsigned char    flag    The flag to append.
 *    @param unsigned int  *len     The length of the buffer.
 *    @param unsigned int   flags   The write flags.
 *    @param unsigned int   check   The flag to check.
 *    @param unsigned int  *ret     The return value.
 */
void append_u8(char **buf, unsigned char flag, unsigned int *len, unsigned int flags, unsigned int check, unsigned int *ret) {
    if (buf == (char **)0x0) {
        LOGF_ERR("buffer is NULL\n");
        (*ret) = 1;
        return;
    }
    if (len == (unsigned int *)0x0) {
        LOGF_ERR("length is NULL\n");
        (*ret) = 1;
        return;
    }
    if (flags & check) {
        (*buf)[*len] = flag;
        ++(*len);
    }
}

/*
 *    Appends a unsigned short to the buffer.
 *
 *    @param char  **buf     The buffer to append to.
 *    @param unsigned short   flag    The flag to append.
 *    @param unsigned int  *len     The length of the buffer.
 *    @param unsigned int   flags   The write flags.
 *    @param unsigned int   check   The flag to check.
 *    @param unsigned int  *ret     The return value.
 */
void append_u16(char **buf, unsigned short flag, unsigned int *len, unsigned int flags, unsigned int check, unsigned int *ret) {
    if (buf == (char **)0x0) {
        LOGF_ERR("buffer is NULL\n");
        (*ret) = 1;
        return;
    }
    if (len == (unsigned int *)0x0) {
        LOGF_ERR("length is NULL\n");
        (*ret) = 1;
        return;
    }
    if (flags & check) {
        (*buf)[*len]     = flag & 0xFF;
        (*buf)[*len + 1] = (flag >> 8) & 0xFF;
        (*len) += 2;
    }
}

/*
 *    Frees the list of tiles in the world.
 *
 *    @param wld_t *wld    The world to free the tiles from.
 */
void free_tiles(wld_t *wld) {
    if (wld == (wld_t *)0x0)
        LOGF_WARN("world is NULL\n");

    int x;
    for (x = 0; x < wld->header.width; ++x)
        free(wld->tiles[x]);

    free(wld->tiles);
}

/*
 *    Dumps the tiles to a png.
 *
 *    @param wld_t *wld    The world to dump the tiles from.
 *    @param char *path    The path to the image to dump to.
 *
 *    @return unsigned int    1 on success, 0 on failure.
 */
unsigned int dump_tiles_png(wld_t *wld, char *path) {
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("world is NULL\n");
        return 0;
    }

    if (path == (char *)0x0) {
        LOGF_ERR("path is NULL\n");
        return 0;
    }

    size_t len = sizeof(unsigned char) * wld->header.width * wld->header.height * 4;

    unsigned char *pBuf = (unsigned char *)malloc(len);
    if (pBuf == (unsigned char *)0x0) {
        LOGF_ERR("failed to allocate memory for image\n");
        return 0;
    }

    int x;
    int y;
    for (x = 0; x < wld->header.width; ++x) {
        for (y = 0; y < wld->header.height; ++y) {
            pBuf[(y * wld->header.width + x) * 4 + 0] = wld->tiles[x][y].tile;
            pBuf[(y * wld->header.width + x) * 4 + 1] = wld->tiles[x][y].tile;
            pBuf[(y * wld->header.width + x) * 4 + 2] = wld->tiles[x][y].tile;
            pBuf[(y * wld->header.width + x) * 4 + 3] = 255;
        }
    }

    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);

    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    struct spng_ihdr ihdr = {0};

    ihdr.width      = wld->header.width;
    ihdr.height     = wld->header.height;
    ihdr.bit_depth  = 8;
    ihdr.color_type = 6;

    spng_set_ihdr(ctx, &ihdr);

    int ret;
    if ((ret = spng_encode_image(ctx, pBuf, len, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE))) {
        VLOGF_ERR("spng_encode_image() failed: %s\n", spng_strerror(ret));
        free(pBuf);
        return 0;
    }

    FILE *fp = fopen(path, "wb");
    if (fp == (FILE *)0x0) {
        VLOGF_ERR("failed to open file for writing: %s\n", path);
        free(pBuf);
        return 0;
    }

    void *pData = spng_get_png_buffer(ctx, &len, &ret);

    if (pData == (void *)0x0) {
        VLOGF_ERR("spng_get_png_buffer() failed: %s\n", spng_strerror(ret));
        free(pBuf);
        return 0;
    }

    fwrite(pData, 1, len, fp);

    fclose(fp);

    free(pData);
    free(pBuf);

    return 1;
}