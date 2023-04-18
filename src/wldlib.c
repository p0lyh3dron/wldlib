/*
 *    wldlib.c    --    Source file for the WLD library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Source file for the WLD library, which contains all the
 *    functions definitions for parsing Terraria's world format.
 */
#include "wldlib.h"
#include "log.h"
#include "parseutil.h"
#include "tilefuncs.h"
#include "wldheaderfuncs.h"
#include "worldgen.h"
#include "rand.h"

#include <malloc.h>
#include <stdio.h>

/*
 *    Loads the chests from a world.
 *
 *    @param wld_t *wld    The world to load the chests from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_chests(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[2];

    short chest_count = 0;
    short item_count  = 0;
    PARSE(buf, pos, short, chest_count);
    PARSE(buf, pos, short, item_count);

    wld->chests = (chest_t *)malloc(sizeof(chest_t) * chest_count);
    if (wld->chests == (chest_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for chests.\n");
        return 0;
    }

    short i;
    for (i = 0; i < chest_count; ++i) {
        chest_t *chest = &wld->chests[i];

        chest->items = (item_t *)malloc(sizeof(item_t) * item_count);

        if (chest->items == (item_t *)0x0) {
            LOGF_ERR("Failed to allocate memory for chest items.\n");
            return 0;
        }

        PARSE(buf, pos, int, chest->x);
        PARSE(buf, pos, int, chest->y);
        chest->name = parse_string(buf, &pos);
        
        short j;
        for (j = 0; j < item_count && j < 40; ++j) {
            short stack = 0;
            PARSE(buf, pos, short, stack);

            if (stack == 0) {
                chest->items[j].stack = 0;
                chest->items[j].id    = 0;
                chest->items[j].prefix = 0;
                continue;
            }

            chest->items[j].stack = stack;
            PARSE(buf, pos, int, chest->items[j].id);
            PARSE(buf, pos, unsigned char, chest->items[j].prefix);
        }

        for (j = 0; j < item_count - 40; ++j) {
            pos += 7;
        }
    }

    if (pos != wld->info.sections[3]) {
        LOGF_WARN("Chest section size mismatch.\n");
        return 0;
    }

    wld->chest_count = chest_count;
}

/*
 *    Writes the chest data to a buffer.
 *
 *    @param wld_t         *wld    The world to write the chest data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the chest data.
 */
char *write_chests(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for chest buffer.\n");
        return (char *)0x0;
    }

    short chest_count = wld->chest_count;
    short item_count  = 40;
    WRITE(buf, pos, short, chest_count);
    WRITE(buf, pos, short, item_count);

    short i;
    for (i = 0; i < chest_count; ++i) {
        chest_t *chest = &wld->chests[i];

        WRITE(buf, pos, int, chest->x);
        WRITE(buf, pos, int, chest->y);
        
        if (chest->name == (char *)0x0) {
            WRITE(buf, pos, unsigned char, 0);
        } else {
            WRITE(buf, pos, unsigned char, strlen(chest->name));
            WRITE_ARRAY(buf, pos, char, chest->name, strlen(chest->name));
        }

        short j;
        for (j = 0; j < item_count; ++j) {
            item_t *item = &chest->items[j];

            if (item->stack == 0) {
                WRITE(buf, pos, short, 0);
                continue;
            }

            WRITE(buf, pos, short, item->stack);
            WRITE(buf, pos, int, item->id);
            WRITE(buf, pos, unsigned char, item->prefix);
        }
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the signs from a world.
 *
 *    @param wld_t *wld    The world to get the signs from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_signs(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[3];

    short sign_count = 0;
    PARSE(buf, pos, short, sign_count);

    wld->signs = (sign_t *)malloc(sizeof(sign_t) * sign_count);
    if (wld->signs == (sign_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for signs.\n");
        return 0;
    }

    short i;
    for (i = 0; i < sign_count; ++i) {
        sign_t *sign = &wld->signs[i];

        sign->text = parse_string(buf, &pos);
        PARSE(buf, pos, int, sign->x);
        PARSE(buf, pos, int, sign->y);
    }

    if (pos != wld->info.sections[4]) {
        LOGF_WARN("Sign section size mismatch.\n");
        return 0;
    }

    wld->sign_count = sign_count;
}

/*
 *    Writes the sign data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the sign data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the sign data.
 */
char *write_signs(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for sign buffer.\n");
        return (char *)0x0;
    }

    short sign_count = wld->sign_count;
    WRITE(buf, pos, short, sign_count);

    short i;
    for (i = 0; i < sign_count; ++i) {
        sign_t *sign = &wld->signs[i];

        WRITE(buf, pos, unsigned char, strlen(sign->text));
        WRITE_ARRAY(buf, pos, char, sign->text, strlen(sign->text));
        WRITE(buf, pos, int, sign->x);
        WRITE(buf, pos, int, sign->y);
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the NPCs from a world.
 *
 *    @param wld_t *wld    The world to get the NPCs from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_npcs(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[4];

    if (wld->ver >= 268) {
        int shimmer_count;
        int shimmer;
        PARSE(buf, pos, int, shimmer_count);

        int i;
        for (i = 0; i < shimmer_count; ++i) {
            PARSE(buf, pos, int, shimmer);
        }
    }

    wld->npcs = (npc_t *)malloc(sizeof(npc_t) * 256);

    if (wld->npcs == (npc_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for NPCs.\n");
        return 0;
    }

    short npc_count = 0;
    unsigned char cont;
    PARSE(buf, pos, unsigned char, cont);

    /* Read NPCs.  */
    while (cont) {
        if (wld->ver >= 190) {
            PARSE(buf, pos, int, wld->npcs[npc_count].id);
        } else {
            LOGF_ERR("Unsupported version.\n");
            return 0;
        }

        wld->npcs[npc_count].name = parse_string(buf, &pos);
        PARSE(buf, pos, float, wld->npcs[npc_count].x);
        PARSE(buf, pos, float, wld->npcs[npc_count].y);
        PARSE(buf, pos, unsigned char, wld->npcs[npc_count].homeless);
        PARSE(buf, pos, int, wld->npcs[npc_count].home_x);
        PARSE(buf, pos, int, wld->npcs[npc_count].home_y);

        unsigned char variant;
        PARSE(buf, pos, unsigned char, variant);

        if (wld->ver >= 213 && (variant & (1 << 0))) {
            PARSE(buf, pos, int, wld->npcs[npc_count].variation);
        }

        ++npc_count;
        PARSE(buf, pos, unsigned char, cont);
    }

    if (wld->ver < 140)
        return 1;

    unsigned long pet_count = npc_count;

    PARSE(buf, pos, unsigned char, cont);

    /* Read pets?  */
    while (cont) {
        if (wld->ver >= 190) {
            PARSE(buf, pos, int, wld->npcs[pet_count].id);
        } else {
            LOGF_ERR("Unsupported version.\n");
            return 0;
        }

        PARSE(buf, pos, float, wld->npcs[pet_count].x);
        PARSE(buf, pos, float, wld->npcs[pet_count].y);

        ++pet_count;
        PARSE(buf, pos, unsigned char, cont);
    }

    if (pos != wld->info.sections[5]) {
        LOGF_WARN("NPC section size mismatch.\n");
        return 0;
    }

    wld->npc_count = npc_count;
    wld->pet_count = pet_count;
}

/*
 *    Writes the NPC data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the NPC data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the NPC data.
 */
char *write_npcs(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for NPC buffer.\n");
        return (char *)0x0;
    }

    WRITE(buf, pos, int, 0);

    short npc_count = 0;
    unsigned char cont = wld->npc_count > 0;
    WRITE(buf, pos, unsigned char, cont);

    /* Write NPCs.  */
    while (npc_count < wld->npc_count) {
        WRITE(buf, pos, int, wld->npcs[npc_count].id);
        WRITE(buf, pos, unsigned char, strlen(wld->npcs[npc_count].name));
        WRITE_ARRAY(buf, pos, char, wld->npcs[npc_count].name, strlen(wld->npcs[npc_count].name));
        WRITE(buf, pos, float, wld->npcs[npc_count].x);
        WRITE(buf, pos, float, wld->npcs[npc_count].y);
        WRITE(buf, pos, unsigned char, wld->npcs[npc_count].homeless);
        WRITE(buf, pos, int, wld->npcs[npc_count].home_x);
        WRITE(buf, pos, int, wld->npcs[npc_count].home_y);

        unsigned char variant = 1;
        WRITE(buf, pos, unsigned char, variant);
        WRITE(buf, pos, int, wld->npcs[npc_count].variation);

        ++npc_count;
        cont = npc_count < wld->npc_count;
        WRITE(buf, pos, unsigned char, cont);
    }

    cont = wld->pet_count > wld->npc_count;
    WRITE(buf, pos, unsigned char, cont);

    /* Write pets?  */
    while (cont) {
        if (wld->ver >= 190) {
            WRITE(buf, pos, int, wld->npcs[npc_count].id);
        } else {
            LOGF_ERR("Unsupported version.\n");
            return 0;
        }

        WRITE(buf, pos, float, wld->npcs[npc_count].x);
        WRITE(buf, pos, float, wld->npcs[npc_count].y);

        ++npc_count;
        WRITE(buf, pos, unsigned char, cont);
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the tile entities from a world.
 *
 *    @param wld_t *wld    The world to get the tile entities from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_tile_entities(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[5];

    int tile_entity_count = 0;
    PARSE(buf, pos, int, tile_entity_count);

    wld->tile_entities = (tile_entity_t *)malloc(sizeof(tile_entity_t) * tile_entity_count);
    if (wld->tile_entities == (tile_entity_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for tile entities.\n");
        return 0;
    }

    int i;
    for (i = 0; i < tile_entity_count; ++i) {
        tile_entity_t *tile_entity = &wld->tile_entities[i];

        PARSE(buf, pos, unsigned char, tile_entity->id);
        PARSE(buf, pos, int, tile_entity->inner);
        PARSE(buf, pos, short, tile_entity->x);
        PARSE(buf, pos, short, tile_entity->y);
    }

    if (pos != wld->info.sections[6]) {
        LOGF_WARN("Tile entity section size mismatch.\n");
        return 0;
    }

    wld->tile_entity_count = tile_entity_count;

    return 1;
}

/*
 *    Writes the tile entity data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the tile entity data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the tile entity data.
 */
char *write_tile_entities(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for tile entity buffer.\n");
        return (char *)0x0;
    }

    WRITE(buf, pos, int, wld->tile_entity_count);

    int i;
    for (i = 0; i < wld->tile_entity_count; ++i) {
        tile_entity_t *tile_entity = &wld->tile_entities[i];

        WRITE(buf, pos, unsigned char, tile_entity->id);
        WRITE(buf, pos, int, tile_entity->inner);
        WRITE(buf, pos, short, tile_entity->x);
        WRITE(buf, pos, short, tile_entity->y);
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the pressure plates from a world.
 *
 *    @param wld_t *wld    The world to get the pressure plates from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_pressure_plates(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[6];

    int pressure_plate_count = 0;
    PARSE(buf, pos, int, pressure_plate_count);

    wld->pressure_plates = (pressure_plate_t *)malloc(sizeof(pressure_plate_t) * pressure_plate_count);
    if (wld->pressure_plates == (pressure_plate_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for pressure plates.\n");
        return 0;
    }

    int i;
    for (i = 0; i < pressure_plate_count; ++i) {
        pressure_plate_t *pressure_plate = &wld->pressure_plates[i];

        PARSE(buf, pos, short, pressure_plate->x);
        PARSE(buf, pos, short, pressure_plate->y);
    }

    if (pos != wld->info.sections[7]) {
        LOGF_WARN("Pressure plate section size mismatch.\n");
        return 0;
    }

    wld->pressure_plate_count = pressure_plate_count;

    return 1;
}

/*
 *    Writes the pressure plate data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the pressure plate data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the pressure plate data.
 */
char *write_pressure_plates(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for pressure plate buffer.\n");
        return (char *)0x0;
    }

    WRITE(buf, pos, int, wld->pressure_plate_count);

    int i;
    for (i = 0; i < wld->pressure_plate_count; ++i) {
        pressure_plate_t *pressure_plate = &wld->pressure_plates[i];

        WRITE(buf, pos, short, pressure_plate->x);
        WRITE(buf, pos, short, pressure_plate->y);
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the town elements from a world.
 *
 *    @param wld_t *wld    The world to get the town elements from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_town_elements(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[7];

    int town_element_count = 0;
    PARSE(buf, pos, int, town_element_count);

    wld->town_elements = (town_element_t *)malloc(sizeof(town_element_t) * town_element_count);
    if (wld->town_elements == (town_element_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for town elements.\n");
        return 0;
    }

    int i;
    for (i = 0; i < town_element_count; ++i) {
        town_element_t *town_element = &wld->town_elements[i];

        PARSE(buf, pos, int, town_element->id);
        PARSE(buf, pos, int, town_element->x);
        PARSE(buf, pos, int, town_element->y);
    }

    if (pos != wld->info.sections[8]) {
        LOGF_WARN("Town element section size mismatch.\n");
        return 0;
    }

    wld->town_element_count = town_element_count;

    return 1;
}

/*
 *    Writes the town element data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the town element data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the town element data.
 */
char *write_town_elements(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for town element buffer.\n");
        return (char *)0x0;
    }

    WRITE(buf, pos, int, wld->town_element_count);

    int i;
    for (i = 0; i < wld->town_element_count; ++i) {
        town_element_t *town_element = &wld->town_elements[i];

        WRITE(buf, pos, int, town_element->id);
        WRITE(buf, pos, int, town_element->x);
        WRITE(buf, pos, int, town_element->y);
    }

    *size = pos;

    return buf;
}

/*
 *    Gets the bestiary from a world.
 *
 *    @param wld_t *wld    The world to get the bestiary from.
 * 
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int get_bestiary(wld_t *wld) {
    char          *buf = wld->file->buf;
    unsigned long  pos = wld->info.sections[8];

    int kill_count = 0;
    PARSE(buf, pos, int, kill_count);

    wld->kills = (kill_t *)malloc(sizeof(kill_t) * kill_count);
    
    if (wld->kills == (kill_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for bestiary.\n");
        return 0;
    }

    int i;
    for (i = 0; i < kill_count; ++i) {
        kill_t *kill = &wld->kills[i];

        kill->name = parse_string(buf, &pos);
        PARSE(buf, pos, int, kill->val);
    }

    wld->kill_count = kill_count;

    int tracker_count = 0;
    PARSE(buf, pos, int, tracker_count);

    wld->trackers = (tracker_t *)malloc(sizeof(tracker_t) * tracker_count);
    if (wld->trackers == (tracker_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for bestiary.\n");
        return 0;
    }

    for (i = 0; i < tracker_count; ++i) {
        tracker_t *tracker = &wld->trackers[i];

        tracker->item = parse_string(buf, &pos);
    }

    wld->tracker_count = tracker_count;

    int chatter_count = 0;
    PARSE(buf, pos, int, chatter_count);

    wld->chatters = (chatted_t *)malloc(sizeof(chatted_t) * chatter_count);

    if (wld->chatters == (chatted_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for bestiary.\n");
        return 0;
    }

    for (i = 0; i < chatter_count; ++i) {
        chatted_t *chatter = &wld->chatters[i];

        chatter->item = parse_string(buf, &pos);
    }

    if (pos != wld->info.sections[9]) {
        LOGF_WARN("Bestiary section size mismatch.\n");
        return 0;
    }

    wld->chatter_count = chatter_count;

    return 1;
}

/*
 *    Writes the bestiary data to a buffer.
 *
 *    @param wld_t         *wld     The world to write the bestiary data from.
 *    @param unsigned long *size    The length of the buffer.
 * 
 *    @return char *    The buffer containing the bestiary data.
 */
char *write_bestiary(wld_t *wld, unsigned long *size) {
    char *buf = (char *)malloc(0x10000);
    unsigned long pos = 0;

    if (buf == (char *)0x0) {
        LOGF_ERR("Failed to allocate memory for bestiary buffer.\n");
        return (char *)0x0;
    }

    WRITE(buf, pos, int, wld->kill_count);

    int i;
    for (i = 0; i < wld->kill_count; ++i) {
        kill_t *kill = &wld->kills[i];

        WRITE(buf, pos, unsigned char, strlen(kill->name));
        WRITE_ARRAY(buf, pos, char, kill->name, strlen(kill->name));
        WRITE(buf, pos, int, kill->val);
    }

    WRITE(buf, pos, int, wld->tracker_count);

    for (i = 0; i < wld->tracker_count; ++i) {
        tracker_t *tracker = &wld->trackers[i];

        WRITE(buf, pos, unsigned char, strlen(tracker->item));
        WRITE_ARRAY(buf, pos, char, tracker->item, strlen(tracker->item));
    }

    WRITE(buf, pos, int, wld->chatter_count);

    for (i = 0; i < wld->chatter_count; ++i) {
        chatted_t *chatter = &wld->chatters[i];

        WRITE(buf, pos, unsigned char, strlen(chatter->item));
        WRITE_ARRAY(buf, pos, char, chatter->item, strlen(chatter->item));
    }

    *size = pos;

    return buf;
}

/*
 *    Creates a new Terraria world.
 *
 *    @param int width           The width of the world.
 *    @param int height          The height of the world.
 *    @param const char *name    The name of the world.
 *    @param const char *seed    The seed of the world.
 * 
 *    @return wld_t *    The created world, or NULL on failure.
 */
wld_t *wld_new(int width, int height, const char *name, const char *seed) {
    wld_t *wld = (wld_t *)malloc(sizeof(wld_t));

    if (wld == (wld_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for world.\n");
        return (wld_t *)0x0;
    }

    int seed_int;

    if (atoi(seed) != 0) {
        seed_int = atoi(seed);
    } else {
        seed_int = rand_crc32(seed, strlen(seed));
    }

    wld->file = (filestream_t *)0x0;

    wld->chest_count = 0;
    wld->chests = (chest_t *)0x0;
    wld->sign_count = 0;
    wld->signs = (sign_t *)0x0;
    wld->npc_count = 0;
    wld->npcs = (npc_t *)0x0;
    wld->kill_count = 0;
    wld->pet_count = 0;
    wld->tile_entity_count = 0;
    wld->tile_entities = (tile_entity_t *)0x0;
    wld->tracker_count = 0;
    wld->trackers = (tracker_t *)0x0;
    wld->chatter_count = 0;
    wld->chatters = (chatted_t *)0x0;
    wld->pressure_plate_count = 0;
    wld->pressure_plates = (pressure_plate_t *)0x0;
    wld->town_element_count = 0;
    wld->town_elements = (town_element_t *)0x0;

    wld->info.ver = 279;
    memcpy(wld->info.sig, "relogic", 7);

    wld->header.name   = name;
    wld->header.seed   = seed;
    wld->header.width  = width;
    wld->header.height = height;

    wld->tiles = (tile_t **)malloc(sizeof(tile_t*) * width);

    if (wld->tiles == (tile_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for tiles.\n");
        free(wld);
        return (wld_t *)0x0;
    }

    unsigned long i;
    for (i = 0; i < width; ++i) {
        wld->tiles[i] = (tile_t*)malloc(sizeof(tile_t) * height);

        if (wld->tiles[i] == (tile_t*)0x0) {
            LOGF_ERR("Failed to allocate memory for tiles.\n");
            free(wld->tiles);
            free(wld);
            return (wld_t *)0x0;
        }
    }

    unsigned long x;
    unsigned long y;
    for (x = 0; x < width; ++x) {
        for (y = 0; y < height; ++y) {
            wld->tiles[x][y].tile = -1;
            wld->tiles[x][y].wall = -1;
            wld->tiles[x][y].liquid_amount = 0;
        }
    }

    wld_gen_world(wld, seed_int, name, width, height);

    return wld;
}

/*
 *    Loads a terraria world.
 *
 *    @param char *path    The file to load.
 *
 *    @return wld_t *    The loaded world, or NULL on failure.
 */
wld_t *wld_open(const char *path) {
    wld_t *wld = (wld_t *)malloc(sizeof(wld_t));
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("Failed to allocate memory for world.\n");
        return (wld_t *)0x0;
    }

    filestream_t *pStream = filestream_open(path);

    if (pStream == (filestream_t *)0x0) {
        LOGF_ERR("Failed to open file.\n");
        free(wld);
        return (wld_t *)0x0;
    }

    wld->file = pStream;

    if (wld_decude_parsing_type(wld) == 0) {
        LOGF_FAT("Failed to decode parsing type.\n");
        return (wld_t *)0x0;
    }

    get_tiles(wld);
    get_chests(wld);
    get_signs(wld);
    get_npcs(wld);

    if (wld->ver >= 116) {
        if (wld->ver < 122) {
           VLOGF_ERR("World version %d is not supported.\n", wld->ver);
        }
        get_tile_entities(wld);
    }

    if (wld->ver >= 170) {
        get_pressure_plates(wld);
    }

    if (wld->ver >= 189) {
        get_town_elements(wld);
    }

    if (wld->ver >= 210) {
        get_bestiary(wld);
    }
    
    wld->creative_powers_len = 31;
    wld->creative_powers = "\001\000\000\000\001\b\000\000\000\000\000\001\t\000\000\001\n\000\000\001\f\000\000\000\000\000\001\r\000\000\000";
    dump_tiles_png(wld, "tiles.png");

    return wld;
}

/*
 *    Writes a world to a file.
 *
 *    @param wld_t *wld    The world to write.
 *    @param char *path      The file to write to.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_write(wld_t *wld, const char *path) {
    FILE *pFile = fopen(path, "wb");

    if (pFile == (FILE *)0x0) {
        LOGF_ERR("Failed to open file.\n");
        return 0;
    }

    unsigned long pos = 0;

    char *sections[11];
    unsigned int section_sizes[11];
    char  footer[0x1000];

    sprintf(footer, "\001%c%s", strlen(wld->header.name), wld->header.name);
    memcpy(footer + 2 + strlen(wld->header.name), &wld->header.id, sizeof(wld->header.id));

    sections[0] = wld_info_get_header(wld, &section_sizes[0]);
    sections[1] = wld_header_get_header(wld, &section_sizes[1]);
    sections[2] = tile_get_buffer(wld, &section_sizes[2]);
    sections[3] = write_chests(wld, &section_sizes[3]);
    sections[4] = write_signs(wld, &section_sizes[4]);
    sections[5] = write_npcs(wld, &section_sizes[5]);
    sections[6] = write_tile_entities(wld, &section_sizes[6]);
    sections[7] = write_pressure_plates(wld, &section_sizes[7]);
    sections[8] = write_town_elements(wld, &section_sizes[8]);
    sections[9] = write_bestiary(wld, &section_sizes[9]);
    sections[10] = "\001\000\000\000\001\b\000\000\000\000\000\001\t\000\000\001\n\000\000\001\f\000\000\000\000\000\001\r\000\000\000";
    section_sizes[10]  = 31;
    sections[11] = footer;
    section_sizes[11] = 2 + strlen(wld->header.name) + 4;

    unsigned int total_size = 0;

    int i;
    for (i = 0; i <= 11; ++i) {
        total_size += section_sizes[i];
        wld->info.sections[i] = total_size;
    }

    sections[0] = wld_info_get_header(wld, &section_sizes[0]);

    for (i = 0; i <= 11; ++i)
        fwrite(sections[i], 1, section_sizes[i], pFile);

    fclose(pFile);

    return 1;
}

/*
 *    Frees a world.
 *
 *    @param wld_t *wld    The world to free.
 */
void wld_free(wld_t *wld) {
    if (wld == (wld_t *)0x0) {
        LOGF_WARN("World is NULL.\n");
        return;
    }

    if (wld->file != (filestream_t *)0x0)
        filestream_free(wld->file);

    free(wld->signs);

    short i;
    for (i = 0; i < wld->chest_count; ++i) {
        free(wld->chests[i].items);
    }

    free(wld->chests);

    free_tiles(wld);
    wld_header_free(wld->header);
    wld_info_header_free(wld->info);
    free(wld);
}