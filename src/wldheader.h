/*
 *    wldheader.h    --    Header file for the WLD file header
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Header file for the WLD file header, which contains all the
 *    structures for the header section of the WLD file.
 *
 *    This is the first section of the file, and contains the version
 *    number, as well as various other information about the world.
 */
#pragma once

#include "types.h"

typedef struct {
    int   ver;
    char  sig[7];
    char  world_type;
    int   revisions;
    long  favorite;
    short numsections;
    int  *sections;
    short tilemask;
    char *uvs;
} wld_info_header_t;

typedef struct {
    char         *name;
    char         *seed;
    long          generator_ver;
    unsigned char guid[16];
    int           id;
    rect_t        bounds;
    int           height;
    int           width;
    int           gamemode;
    unsigned char drunk;
    unsigned char ftw;
    unsigned char expert;
    unsigned char master;
    long          creation_time;
    unsigned char moon_type;
    int           tree_x[3];
    int           tree_styles[4];
    int           cave_back_x[3];
    int           cave_back_style[4];
    int           ice_back_style;
    int           jungle_back_style;
    int           hell_back_style;
    int           spawn_x;
    int           spawn_y;
    double        ground_level;
    double        rock_level;
    double        time;
    unsigned char day;
    int           moon_phase;
    unsigned char blood_moon;
    unsigned char eclipse;
    int           dungeon_x;
    int           dungeon_y;
    unsigned char crimson;
    unsigned char kill_eoc;
    unsigned char kill_evil_boss;
    unsigned char kill_skeletron;
    unsigned char kill_queen_bee;
    unsigned char kill_destroyer;
    unsigned char kill_twins;
    unsigned char kill_skeletron_prime;
    unsigned char kill_hm_boss;
    unsigned char kill_plantera;
    unsigned char kill_golem;
    unsigned char kill_king_slime;
    unsigned char saved_tinkerer;
    unsigned char saved_wizard;
    unsigned char saved_mechanic;
    unsigned char kill_goblin;
    unsigned char kill_clown;
    unsigned char kill_frost;
    unsigned char kill_pirate;
    unsigned char broke_orb;
    unsigned char meteor;
    unsigned char orb_smashed;
    int           altar_count;
    unsigned char hardmode;
    int           invasion_delay;
    int           invasion_size;
    int           invasion_type;
    double        invasion_x;
    double        slime_rain_time;
    unsigned char sundial_cooldown;
    unsigned char is_raining;
    int           rain_time;
    float         max_rain;
    int           ore_tier_1;
    int           ore_tier_2;
    int           ore_tier_3;
    unsigned char tree_style;
    unsigned char corruption_style;
    unsigned char jungle_style;
    unsigned char snow_style;
    unsigned char hallow_style;
    unsigned char crimson_style;
    unsigned char desert_style;
    unsigned char ocean_style;
    int           cloud_bg;
    short         num_clouds;
    float         wind_speed;
    int           players;
    char        **playernames;
    unsigned char saved_angler;
    int           angler_quest;
    unsigned char saved_stylist;
    unsigned char saved_tax_collector;
    unsigned char saved_golfer;
    int           invasion_start_size;
    int           cultist_delay;
    short         kill_count_len;
    int          *kill_counts;
    unsigned char fast_forward_time;
    unsigned char kill_fishron;
    unsigned char kill_martian;
    unsigned char kill_cultist;
    unsigned char kill_moonlord;
    unsigned char kill_pumpking;
    unsigned char kill_wood;
    unsigned char kill_ice_queen;
    unsigned char kill_tank;
    unsigned char kill_everscream;
    unsigned char kill_solar;
    unsigned char kill_vortex;
    unsigned char kill_nebula;
    unsigned char kill_stardust;
    unsigned char active_solar;
    unsigned char active_vortex;
    unsigned char active_nebula;
    unsigned char active_stardust;
    unsigned char active_lunar;
    unsigned char manual_party;
    unsigned char invite_party;
    int           party_cooldown;
    int           partier_len;
    int          *partiers;
    unsigned char active_sandstorm;
    int           sandstorm_time;
    float         sandstorm_severity;
    float         sandstorm_max_severity;
    unsigned char saved_bartender;
    unsigned char kill_dd2_1;
    unsigned char kill_dd2_2;
    unsigned char kill_dd2_3;
    unsigned char style_8;
    unsigned char style_9;
    unsigned char style_10;
    unsigned char style_11;
    unsigned char style_12;
    unsigned char combat_book;
    int           lantern_night_cooldown;
    unsigned char lantern_night;
    unsigned char manual_lantern_night;
    unsigned char next_lantern_real;
    int           tree_tops_len;
    int          *tree_tops;
    unsigned char forced_halloween;
    unsigned char forced_christmas;
    int           copper_id;
    int           iron_id;
    int           silver_id;
    int           gold_id;
    unsigned char bought_cat;
    unsigned char bought_dog;
    unsigned char bought_bunny;
    unsigned char kill_eol;
    unsigned char kill_queen_slime;
} wld_header_t;