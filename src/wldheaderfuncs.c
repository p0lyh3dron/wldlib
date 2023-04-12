/*
 *    wldheaderfuncs.c    --    Source file for the WLD file header
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Source file for parsing the WLD file header.
 */
#include "wldheaderfuncs.h"

#include "log.h"

#include "parseutil.h"
#include "wldheader.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define WLD_INFO_HEADER_LEN 0xFF
#define WLD_HEADER_LEN      0xFFFF

/*
 *    Peeks at the world header and returns the version of the world.
 *    Returns -1 if the world is invalid.
 *
 *    @param wld_t *wld    The world to check.
 *
 *    @return int          The version of the world.
 */
int wld_get_version(wld_t *wld) {
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("World is NULL.\n");
        return -1;
    }

    if (wld->file == (filestream_t *)0x0) {
        LOGF_ERR("World file stream is NULL.\n");
        return -1;
    }

    if (wld->file->buf == (unsigned char *)0x0) {
        LOGF_ERR("World file buffer is NULL.\n");
        return -1;
    }

    return *(int *)wld->file->buf;
}

/*
 *    Parses the world info header.
 *
 *    @param wld_t* wld    The world to parse.
 *
 *    @return unsigned int          1 on success, 0 on failure.
 */
unsigned int wld_info_parse(wld_t *wld) {
    if (wld == (wld_t *)0x0) {
        LOGF_ERR("World is NULL.\n");
        return 0;
    }

    unsigned char *buf = wld->file->buf;
    unsigned int  *pos = &wld->file->pos;

    PARSE(buf, *pos, unsigned int, wld->info.ver);
    PARSE_ARRAY(buf, *pos, unsigned char, wld->info.sig, 7);
    PARSE(buf, *pos, char, wld->info.world_type);
    PARSE(buf, *pos, int, wld->info.revisions);
    PARSE(buf, *pos, long, wld->info.favorite);
    PARSE(buf, *pos, short, wld->info.numsections);

    wld->info.sections = (int *)malloc(sizeof(int) * wld->info.numsections);

    if (wld->info.sections == (int *)0x0) {
        LOGF_ERR("Failed to allocate memory for sections.\n");
        return 0;
    }

    PARSE_ARRAY(buf, *pos, int, wld->info.sections, wld->info.numsections);

    PARSE(buf, *pos, short, wld->info.tilemask);

    int bits = 0;

    if (wld->info.tilemask % 8 == 0)
        bits = wld->info.tilemask / 8;
    else
        bits = wld->info.tilemask / 8 + 1;

    wld->info.uvs = (char *)malloc(sizeof(char) * bits);

    if (!wld->info.uvs) {
        LOGF_ERR("Failed to allocate memory for UVs.\n");
        return 0;
    }

    PARSE_ARRAY(buf, *pos, char, wld->info.uvs, bits);
#if DEBUG
    wld_info_header_dump(spWld->info);
#endif /* DEBUG  */

    return 1;
}

/*
 *    Parses the world format header.
 *
 *    @param wld_t* wld    The world to parse.
 *
 *    @return unsigned int    1 on success, 0 on failure.
 */
unsigned int wld_header_parse(wld_t *wld) {
    if (!wld_info_parse(wld)) {
        LOGF_ERR("Failed to parse world info header.\n");
        return 0;
    }

    unsigned char *buf    = wld->file->buf;
    unsigned int  *pos    = &wld->file->pos;
    wld_header_t  *header = &wld->header;

    header->name = parse_string(buf, pos);

    if (wld->ver >= 179) {
        header->seed = parse_string(buf, pos);

        PARSE(buf, *pos, long, header->generator_ver);
    }

    if (wld->ver >= 181)
        PARSE_ARRAY(buf, *pos, unsigned char, header->guid, 16);

    PARSE(buf, *pos, int, header->id);
    PARSE(buf, *pos, rect_t, header->bounds);
    PARSE(buf, *pos, int, header->height);
    PARSE(buf, *pos, int, header->width);

    if (wld->ver >= 209) {
        PARSE(buf, *pos, int, header->gamemode);

        if (wld->ver >= 222)
            PARSE(buf, *pos, unsigned char, header->drunk);

        if (wld->ver >= 227)
            PARSE(buf, *pos, unsigned char, header->ftw);

        if (wld->ver >= 238)
            PARSE(buf, *pos, unsigned char, header->tenth);

        if (wld->ver >= 239)
            PARSE(buf, *pos, unsigned char, header->dont_starve);

        if (wld->ver >= 241)
            PARSE(buf, *pos, unsigned char, header->bees);
        
        if (wld->ver >= 249)
            PARSE(buf, *pos, unsigned char, header->remix);

        if (wld->ver >= 266)
            PARSE(buf, *pos, unsigned char, header->no_traps);
        
        if (wld->ver >= 267) {
            PARSE(buf, *pos, unsigned char, header->zenith);
        } else {
            header->zenith = header->remix && header->drunk;
        }
    } else {
        if (wld->ver >= 112) {
            PARSE(buf, *pos, unsigned char, header->gamemode);
        } else {
            header->gamemode = 0;
        }

        if (wld->ver >= 208 && buf[*pos] != 0x0)
            header->gamemode = 2;
    }

    if (wld->ver >= 141)
        PARSE(buf, *pos, long, header->creation_time);

    if (wld->ver >= 63)
        PARSE(buf, *pos, unsigned char, header->moon_type);

    if (wld->ver >= 44) {
        PARSE_ARRAY(buf, *pos, int, header->tree_x, 3);
        PARSE_ARRAY(buf, *pos, int, header->tree_styles, 4);
    }

    if (wld->ver >= 60) {
        PARSE_ARRAY(buf, *pos, int, header->cave_back_x, 3);
        PARSE_ARRAY(buf, *pos, int, header->cave_back_style, 4);

        PARSE(buf, *pos, int, header->ice_back_style);
    }

    if (wld->ver >= 61) {
        PARSE(buf, *pos, int, header->jungle_back_style);
        PARSE(buf, *pos, int, header->hell_back_style);
    }

    PARSE(buf, *pos, int, header->spawn_x);
    PARSE(buf, *pos, int, header->spawn_y);
    PARSE(buf, *pos, double, header->ground_level);
    PARSE(buf, *pos, double, header->rock_level);
    PARSE(buf, *pos, double, header->time);
    PARSE(buf, *pos, unsigned char, header->day);
    PARSE(buf, *pos, int, header->moon_phase);
    PARSE(buf, *pos, unsigned char, header->blood_moon);

    if (wld->ver >= 63)
        PARSE(buf, *pos, unsigned char, header->eclipse);

    PARSE(buf, *pos, int, header->dungeon_x);
    PARSE(buf, *pos, int, header->dungeon_y);

    if (wld->ver >= 56)
        PARSE(buf, *pos, unsigned char, header->crimson);

    PARSE(buf, *pos, unsigned char, header->kill_eoc);
    PARSE(buf, *pos, unsigned char, header->kill_evil_boss);
    PARSE(buf, *pos, unsigned char, header->kill_skeletron);

    if (wld->ver >= 66)
        PARSE(buf, *pos, unsigned char, header->kill_queen_bee);

    if (wld->ver >= 44) {
        PARSE(buf, *pos, unsigned char, header->kill_destroyer);
        PARSE(buf, *pos, unsigned char, header->kill_twins);
        PARSE(buf, *pos, unsigned char, header->kill_skeletron_prime);
        PARSE(buf, *pos, unsigned char, header->kill_hm_boss);
    }

    if (wld->ver >= 64) {
        PARSE(buf, *pos, unsigned char, header->kill_plantera);
        PARSE(buf, *pos, unsigned char, header->kill_golem);
    }

    if (wld->ver >= 118)
        PARSE(buf, *pos, unsigned char, header->kill_king_slime);

    if (wld->ver >= 29) {
        PARSE(buf, *pos, unsigned char, header->saved_tinkerer);
        PARSE(buf, *pos, unsigned char, header->saved_wizard);
    }

    if (wld->ver >= 34)
        PARSE(buf, *pos, unsigned char, header->saved_mechanic);

    if (wld->ver >= 29)
        PARSE(buf, *pos, unsigned char, header->kill_goblin);

    if (wld->ver >= 32)
        PARSE(buf, *pos, unsigned char, header->kill_clown);

    if (wld->ver >= 37)
        PARSE(buf, *pos, unsigned char, header->kill_frost);

    if (wld->ver >= 56)
        PARSE(buf, *pos, unsigned char, header->kill_pirate);

    PARSE(buf, *pos, unsigned char, header->broke_orb);
    PARSE(buf, *pos, unsigned char, header->meteor);
    PARSE(buf, *pos, unsigned char, header->orb_smashed);

    if (wld->ver >= 23) {
        PARSE(buf, *pos, int, header->altar_count);
        PARSE(buf, *pos, unsigned char, header->hardmode);
    }

    if (wld->ver >= 257)
        PARSE(buf, *pos, unsigned char, header->after_doom_party);

    PARSE(buf, *pos, int, header->invasion_delay);
    PARSE(buf, *pos, int, header->invasion_size);
    PARSE(buf, *pos, int, header->invasion_type);
    PARSE(buf, *pos, double, header->invasion_x);

    if (wld->ver >= 118)
        PARSE(buf, *pos, double, header->slime_rain_time);

    if (wld->ver >= 113)
        PARSE(buf, *pos, unsigned char, header->sundial_cooldown);

    if (wld->ver >= 53) {
        PARSE(buf, *pos, unsigned char, header->is_raining);
        PARSE(buf, *pos, int, header->rain_time);
        PARSE(buf, *pos, float, header->max_rain);
    }

    if (wld->ver >= 54) {
        PARSE(buf, *pos, int, header->ore_tier_1);
        PARSE(buf, *pos, int, header->ore_tier_2);
        PARSE(buf, *pos, int, header->ore_tier_3);
    }

    if (wld->ver >= 55) {
        PARSE(buf, *pos, unsigned char, header->tree_style);
        PARSE(buf, *pos, unsigned char, header->corruption_style);
        PARSE(buf, *pos, unsigned char, header->jungle_style);
    }

    if (wld->ver >= 60) {
        PARSE(buf, *pos, unsigned char, header->snow_style);
        PARSE(buf, *pos, unsigned char, header->hallow_style);
        PARSE(buf, *pos, unsigned char, header->crimson_style);
        PARSE(buf, *pos, unsigned char, header->desert_style);
        PARSE(buf, *pos, unsigned char, header->ocean_style);
        PARSE(buf, *pos, int, header->cloud_bg);
    }

    if (wld->ver >= 62) {
        PARSE(buf, *pos, short, header->num_clouds);
        PARSE(buf, *pos, float, header->wind_speed);
    }

    if (wld->ver >= 95) {
        PARSE(buf, *pos, int, header->players);

        header->playernames = (char **)malloc(sizeof(char *) * header->players);
        int i;

        for (i = 0; i < header->players; i++)
            header->playernames[i] = parse_string(buf, pos);
    }

    if (wld->ver >= 99)
        PARSE(buf, *pos, unsigned char, header->saved_angler);

    if (wld->ver >= 101)
        PARSE(buf, *pos, int, header->angler_quest);

    if (wld->ver >= 104)
        PARSE(buf, *pos, unsigned char, header->saved_stylist);

    if (wld->ver >= 129)
        PARSE(buf, *pos, unsigned char, header->saved_tax_collector);

    if (wld->ver >= 201)
        PARSE(buf, *pos, unsigned char, header->saved_golfer);

    if (wld->ver >= 107)
        PARSE(buf, *pos, int, header->invasion_start_size);

    if (wld->ver >= 108)
        PARSE(buf, *pos, int, header->cultist_delay);

    if (wld->ver >= 109) {
        PARSE(buf, *pos, short, header->kill_count_len);

        header->kill_counts = (int *)malloc(sizeof(int) * header->kill_count_len);
        if (header->kill_counts == (int *)0x0) {
            LOGF_ERR("Failed to allocate memory for kill counts\n");
            return 0;
        }

        PARSE_ARRAY(buf, *pos, int, header->kill_counts, header->kill_count_len);
    }

    if (wld->ver >= 128)
        PARSE(buf, *pos, unsigned char, header->fast_forward_time);

    if (wld->ver >= 131) {
        PARSE(buf, *pos, unsigned char, header->kill_fishron);
        PARSE(buf, *pos, unsigned char, header->kill_martian);
        PARSE(buf, *pos, unsigned char, header->kill_cultist);
        PARSE(buf, *pos, unsigned char, header->kill_moonlord);
        PARSE(buf, *pos, unsigned char, header->kill_pumpking);
        PARSE(buf, *pos, unsigned char, header->kill_wood);
        PARSE(buf, *pos, unsigned char, header->kill_ice_queen);
        PARSE(buf, *pos, unsigned char, header->kill_tank);
        PARSE(buf, *pos, unsigned char, header->kill_everscream);
    }

    if (wld->ver >= 140) {
        PARSE(buf, *pos, unsigned char, header->kill_solar);
        PARSE(buf, *pos, unsigned char, header->kill_vortex);
        PARSE(buf, *pos, unsigned char, header->kill_nebula);
        PARSE(buf, *pos, unsigned char, header->kill_stardust);
        PARSE(buf, *pos, unsigned char, header->active_solar);
        PARSE(buf, *pos, unsigned char, header->active_vortex);
        PARSE(buf, *pos, unsigned char, header->active_nebula);
        PARSE(buf, *pos, unsigned char, header->active_stardust);
        PARSE(buf, *pos, unsigned char, header->active_lunar);
    }

    if (wld->ver >= 170) {
        PARSE(buf, *pos, unsigned char, header->manual_party);
        PARSE(buf, *pos, unsigned char, header->invite_party);
        PARSE(buf, *pos, int, header->party_cooldown);
        PARSE(buf, *pos, int, header->partier_len);

        header->partiers = (int *)malloc(sizeof(int) * header->partier_len);
        if (header->partiers == (int *)0x0) {
            LOGF_ERR("Failed to allocate memory for partiers\n");
            return 0;
        }
        PARSE_ARRAY(buf, *pos, int, header->partiers, header->partier_len);
    }

    if (wld->ver >= 174) {
        PARSE(buf, *pos, unsigned char, header->active_sandstorm);
        PARSE(buf, *pos, int, header->sandstorm_time);
        PARSE(buf, *pos, float, header->sandstorm_severity);
        PARSE(buf, *pos, float, header->sandstorm_max_severity);
    }

    if (wld->ver >= 178) {
        PARSE(buf, *pos, unsigned char, header->saved_bartender);
        PARSE(buf, *pos, unsigned char, header->kill_dd2_1);
        PARSE(buf, *pos, unsigned char, header->kill_dd2_2);
        PARSE(buf, *pos, unsigned char, header->kill_dd2_3);
    }

    if (wld->ver >= 194)
        PARSE(buf, *pos, unsigned char, header->style_8);

    if (wld->ver >= 215)
        PARSE(buf, *pos, unsigned char, header->style_9);

    if (wld->ver >= 195) {
        PARSE(buf, *pos, unsigned char, header->style_10);
        PARSE(buf, *pos, unsigned char, header->style_11);
        PARSE(buf, *pos, unsigned char, header->style_12);
    }

    if (wld->ver >= 204)
        PARSE(buf, *pos, unsigned char, header->combat_book);

    if (wld->ver >= 207) {
        PARSE(buf, *pos, int, header->lantern_night_cooldown);
        PARSE(buf, *pos, unsigned char, header->lantern_night);
        PARSE(buf, *pos, unsigned char, header->manual_lantern_night);
        PARSE(buf, *pos, unsigned char, header->next_lantern_real);
    }

    if (wld->ver >= 211) {
        PARSE(buf, *pos, int, header->tree_tops_len);

        header->tree_tops = (int *)malloc(sizeof(int) * header->tree_tops_len);
        if (header->tree_tops == (int *)0x0) {
            LOGF_ERR("Failed to allocate memory for tree tops\n");
            return 0;
        }

        PARSE_ARRAY(buf, *pos, int, header->tree_tops, header->tree_tops_len);
    }

    if (wld->ver >= 212) {
        PARSE(buf, *pos, unsigned char, header->forced_halloween);
        PARSE(buf, *pos, unsigned char, header->forced_christmas);
    }

    if (wld->ver >= 216) {
        PARSE(buf, *pos, int, header->copper_id);
        PARSE(buf, *pos, int, header->iron_id);
        PARSE(buf, *pos, int, header->silver_id);
        PARSE(buf, *pos, int, header->gold_id);
    }

    if (wld->ver >= 217) {
        PARSE(buf, *pos, unsigned char, header->bought_cat);
        PARSE(buf, *pos, unsigned char, header->bought_dog);
        PARSE(buf, *pos, unsigned char, header->bought_bunny);
    }

    if (wld->ver >= 223) {
        PARSE(buf, *pos, unsigned char, header->kill_eol);
        PARSE(buf, *pos, unsigned char, header->kill_queen_slime);
    }

    if (wld->ver >= 240)
        PARSE(buf, *pos, unsigned char, header->kill_deer);

    if (wld->ver >= 250)
        PARSE(buf, *pos, unsigned char, header->blue_slime);

    if (wld->ver >= 251) {
        PARSE(buf, *pos, unsigned char, header->unlocked_merchant);
        PARSE(buf, *pos, unsigned char, header->unlocked_demo);
        PARSE(buf, *pos, unsigned char, header->unlocked_party);
        PARSE(buf, *pos, unsigned char, header->unlocked_dye);
        PARSE(buf, *pos, unsigned char, header->unlocked_truffle);
        PARSE(buf, *pos, unsigned char, header->unlocked_arms_dealer);
        PARSE(buf, *pos, unsigned char, header->unlocked_nurse);
        PARSE(buf, *pos, unsigned char, header->unlocked_princess);
    }

    if (wld->ver >= 259)
        PARSE(buf, *pos, unsigned char, header->combat_book_2);

    if (wld->ver >= 260)
        PARSE(buf, *pos, unsigned char, header->peddler_satchel);

    if (wld->ver >= 261) {
        PARSE(buf, *pos, unsigned char, header->green_slime);
        PARSE(buf, *pos, unsigned char, header->old_slime);
        PARSE(buf, *pos, unsigned char, header->purple_slime);
        PARSE(buf, *pos, unsigned char, header->rainbow_slime);
        PARSE(buf, *pos, unsigned char, header->red_slime);
        PARSE(buf, *pos, unsigned char, header->yellow_slime);
        PARSE(buf, *pos, unsigned char, header->copper_slime);
    }

    if (wld->ver >= 264) {
        PARSE(buf, *pos, unsigned char, header->moondial_active);
        PARSE(buf, *pos, unsigned char, header->moondial_cooldown);
    }

#if DEBUG
    wld_header_dump(spWld->header);
#endif /* DEBUG  */
    return 1;
}

/*
 *    Returns the world info header as a buffer.
 *
 *    @param wld_t *wld      The world to get the header from.
 *    @param unsigned int   *len      The length of the header.
 *
 *    @return char *           The world info header.
 */
char *wld_info_get_header(wld_t *wld, unsigned int *len) {
    static char buf[WLD_INFO_HEADER_LEN];

    memcpy(buf, &wld->info, WLD_INFO_HEADER_LEN);
    memcpy(buf + sizeof(wld_info_header_t) - sizeof(wld->info.tilemask), wld->info.uvs, wld->info.tilemask / 8);

    *len = sizeof(wld_info_header_t) - sizeof(wld->info.tilemask) + wld->info.tilemask / 8;

    return buf;
}

/*
 *    Returns the world format header as a buffer.
 *
 *    @param wld_t *wld    The world to get the header from.
 *    @param unsigned int   *len    The length of the header.
 *
 *    @return char *         The world format header.
 */
char *wld_header_get_header(wld_t *wld, unsigned int *len) {
    static char pBuf[WLD_HEADER_LEN];

    memcpy(pBuf, &wld->header, WLD_HEADER_LEN);

    *len = sizeof(wld_header_t);

    return pBuf;
}

/*
 *    Dumps the contents of the world format header to stdout.
 *
 *    @param wld_header_t header    The header to dump.
 */
void wld_header_dump(wld_header_t header) {
    printf("World Header:\n");
    printf("    Name:                   %s\n", header.name);
    printf("    Seed:                   %s\n", header.seed);
    printf("    GUID:                   %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
           header.guid[0], header.guid[1], header.guid[2], header.guid[3],
           header.guid[4], header.guid[5], header.guid[6], header.guid[7],
           header.guid[8], header.guid[9], header.guid[10], header.guid[11],
           header.guid[12], header.guid[13], header.guid[14], header.guid[15]);

    printf("    ID:                     %d\n", header.id);
    printf("    Bounds:                 %d, %d, %d, %d\n", header.bounds.x0, header.bounds.x, header.bounds.y0, header.bounds.y);
    printf("    Height:                 %d\n", header.height);
    printf("    Width:                  %d\n", header.width);
    printf("    Game Mode:              %d\n", header.gamemode);
    printf("    Drunk:                  %d\n", header.drunk);
    printf("    FTW:                    %d\n", header.ftw);
    printf("    Creation:               %ld\n", header.creation_time);
    printf("    Moon Type:              %d\n", header.moon_type);
    printf("    Tree X:                 %d, %d, %d\n", header.tree_x[0], header.tree_x[1], header.tree_x[2]);
    printf("    Tree Style:             %d, %d, %d, %d\n", header.tree_styles[0], header.tree_styles[1], header.tree_styles[2], header.tree_styles[3]);
    printf("    Cave X:                 %d, %d, %d\n", header.cave_back_x[0], header.cave_back_x[1], header.cave_back_x[2]);
    printf("    Cave Style:             %d, %d, %d, %d\n", header.cave_back_style[0], header.cave_back_style[1], header.cave_back_style[2], header.cave_back_style[3]);
    printf("    Ice Style:              %d\n", header.ice_back_style);
    printf("    Jungle Style:           %d\n", header.jungle_back_style);
    printf("    Hell Style:             %d\n", header.hell_back_style);
    printf("    Spawn X:                %d\n", header.spawn_x);
    printf("    Spawn Y:                %d\n", header.spawn_y);
    printf("    Ground Level:           %f\n", header.ground_level);
    printf("    Rock Level:             %f\n", header.rock_level);
    printf("    Time:                   %f\n", header.time);
    printf("    Day:                    %d\n", header.day);
    printf("    Moon Phase:             %d\n", header.moon_phase);
    printf("    Blood Moon:             %d\n", header.blood_moon);
    printf("    Eclipse:                %d\n", header.eclipse);
    printf("    Dungeon X:              %d\n", header.dungeon_x);
    printf("    Dungeon Y:              %d\n", header.dungeon_y);
    printf("    Crimson:                %d\n", header.crimson);
    printf("    Killed Eoc:             %d\n", header.kill_eoc);
    printf("    Killed Evil Boss:       %d\n", header.kill_evil_boss);
    printf("    Killed Skeletron:       %d\n", header.kill_skeletron);
    printf("    Killed Queen Bee:       %d\n", header.kill_queen_bee);
    printf("    Killed Destroyer:       %d\n", header.kill_destroyer);
    printf("    Killed Twins:           %d\n", header.kill_twins);
    printf("    Killed Skeletron Prime: %d\n", header.kill_skeletron_prime);
    printf("    Killed Plantera:        %d\n", header.kill_plantera);
    printf("    Killed Golem:           %d\n", header.kill_golem);
    printf("    Killed King Slime:      %d\n", header.kill_king_slime);
    printf("    Saved Tinkerer:         %d\n", header.saved_tinkerer);
    printf("    Saved Wizard:           %d\n", header.saved_wizard);
    printf("    Saved Mechanic:         %d\n", header.saved_mechanic);
    printf("    Defeated Goblins:       %d\n", header.kill_goblin);
    printf("    Killed Clown:           %d\n", header.kill_clown);
    printf("    Killed Pirates:         %d\n", header.kill_pirate);
    printf("    Broke Orb:              %d\n", header.broke_orb);
    printf("    Meteor Fell:            %d\n", header.meteor);
    printf("    Smashed Orbs:           %d\n", header.orb_smashed);
    printf("    Altars Destroyed:       %d\n", header.altar_count);
    printf("    Hardmode:               %d\n", header.hardmode);
    printf("    Invasion Delay:         %d\n", header.invasion_delay);
    printf("    Invasion Size:          %d\n", header.invasion_size);
    printf("    Invasion Type:          %d\n", header.invasion_type);
    printf("    Invasion X:             %f\n", header.invasion_x);
    printf("    Slime Rain Time:        %f\n", header.slime_rain_time);
    printf("    Sundial Cooldown:       %d\n", header.sundial_cooldown);
    printf("    Raining:                %d\n", header.is_raining);
    printf("    Rain Time:              %d\n", header.rain_time);
    printf("    Max Rain:               %f\n", header.max_rain);
    printf("    Tier 1 Ore:             %d\n", header.ore_tier_1);
    printf("    Tier 2 Ore:             %d\n", header.ore_tier_2);
    printf("    Tier 3 Ore:             %d\n", header.ore_tier_3);
    printf("    Tree Style:             %d\n", header.tree_style);
    printf("    Corruption Style:       %d\n", header.corruption_style);
}

/*
 *    Dumps the contents of the world info header to stdout.
 *
 *    @param wld_info_header_t info    The header to dump.
 */
void wld_info_header_dump(wld_info_header_t info) {
    short i;
    printf("World Info Header:\n");
    printf("    Version:      %d\n", info.ver);
    printf("    Signature:    %s\n", info.sig);
    printf("    World Type:   %d\n", info.world_type);
    printf("    Revisions:    %d\n", info.revisions);
    printf("    Favorite:     %ld\n", info.favorite);
    printf("    Num Sections: %d\n", info.numsections);
    printf("    Section Offsets:\n");

    for (i = 0; i < info.numsections; i++)
        printf("        %d: %d\n", i, info.sections[i]);

    printf("\n");
    printf("    Tile Mask: %d\n", info.tilemask);
    printf("    UVs:\n");

    for (i = 0; i < info.tilemask / 8 + 1; i++)
        printf("        %d: %d\n", i, info.uvs[i]);

    printf("\n");
}

/*
 *    Frees a world info header.
 *
 *    @param wld_info_header_t header    The world header to free.
 */
void wld_info_header_free(wld_info_header_t header) {
    if (header.sections)
        free(header.sections);
    if (header.uvs)
        free(header.uvs);
}

/*
 *    Frees a world format header.
 *
 *    @param wld_header_t    header    The world header to free.
 */
void wld_header_free(wld_header_t header) {
    if (header.name)
        free(header.name);

    if (header.seed)
        free(header.seed);

    if (header.playernames) {
        int i;
        for (i = 0; i < header.players; i++) {
            if (header.playernames[i])
                free(header.playernames[i]);
        }

        free(header.playernames);
    }

    if (header.kill_counts)
        free(header.kill_counts);

    if (header.partiers)
        free(header.partiers);

    if (header.tree_tops)
        free(header.tree_tops);
}