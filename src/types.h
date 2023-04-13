/*
 *    types.h    --    Header file for the WLD types
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Header file the types used in the WLD file.
 */
#ifndef WLDLIB_TYPES_H
#define WLDLIB_TYPES_H

typedef struct {
    int x0;
    int x;
    int y0;
    int y;
} rect_t;

typedef struct {
    int   id;
    short stack;
    char  prefix;
} item_t;

typedef struct {
    int     x;
    int     y;
    char   *name;
    item_t *items;
} chest_t;

typedef struct {
    char *text;
    int   x;
    int   y;
} sign_t;

typedef struct {
    unsigned char shimmer;
    int           id;
    char         *name;
    float         x;
    float         y;
    unsigned char homeless;
    int           home_x;
    int           home_y;
    int           variation;
} npc_t;

typedef struct {
    unsigned char id;
    int           inner;
    short         x;
    short         y;
} tile_entity_t;

typedef struct {
    int x;
    int y;
} pressure_plate_t;

typedef struct {
    int id;
    int x;
    int y;
} town_element_t;

typedef struct {
    char *name;
    int   val;
} kill_t;

typedef struct {
    char *item;
} tracker_t;

typedef struct {
    char *item;
} chatted_t;

#endif /* WLDLIB_TYPES_H  */