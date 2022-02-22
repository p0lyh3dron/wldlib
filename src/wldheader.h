/*
 *    wldheader.h    --    Header file for the WLD file header
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
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
    s32  aVer;
    s8   aSig[ 7 ];
    s8   aWorldType;
    s32  aRevisions;
    s64  aFavorite;
    s16  aNumSections;
    s32 *apSections;
    s16  aTileMask;
    s8  *apUVs;
}wld_info_header_t;

typedef struct {
    s8    *apName;
    s8    *apSeed;
    s64    aGeneratorVer;
    u8     aGuid[ 16 ];
    s32    aId;
    rect_t aBounds;
    s32    aHeight;
    s32    aWidth;
    s32    aGameMode;
    u8     aDrunk;
    u8     aFtw;
    u8     aExpert;
    u8     aMaster;
    s64    aCreationTime;
    u8     aMoonType;
    s32    aTreeX[ 3 ];
    s32    aTreeStyles[ 4 ];
    s32    aCaveBackX[ 3 ];
    s32    aCaveBackStyle[ 4 ];
    s32    aIceBackStyle;
    s32    aJungleBackStyle;
    s32    aHellBackStyle;
    s32    aSpawnX;
    s32    aSpawnY;
    f64    aGroundLevel;
    f64    aRockLevel;
    f64    aTime;
    u8     aDay;
    s32    aMoonPhase;
    u8     aBloodMoon;
    u8     aIsEclipse;
    s32    aDungeonX;
    s32    aDungeonY;
    u8     aIsCrimson;
    u8     aKilledEoc;
    u8     aKilledEvilBoss;
    u8     aKilledSkeletron;
    u8     aKilledQueenBee;
    u8     aKilledDestroyer;
    u8     aKilledTwins;
    u8     aKilledSkeletronPrime;
    u8     aKilledHMBoss;
    u8     aKilledPlantBoss;
    u8     aKilledGolemBoss;
    u8     aKilledKingSlime;
    u8     aSavedTinkerer;
    u8     aSavedWizard;
    u8     aSavedMechanic;
    u8     aDefeatedGoblins;
    u8     aKilledClown;
    u8     aKilledFrost;
    u8     aKilledPirates;
    u8     aBrokeOrb;
    u8     aMeteor;
    u8     aShadowOrbSmashed;
    s32    aAltarCount;
    u8     aHardMode;
    s32    aInvasionDelay;
    s32    aInvasionSize;
    s32    aInvasionType;
    f64    aInvasionX;
    f64    aSlimeRainTime;
    u8     aSundialCooldown;
    u8     aIsRaining;
    s32    aRainTime;
    f32    aMaxRain;
    s32    aOreTier1;
    s32    aOreTier2;
    s32    aOreTier3;
    u8     aTreeStyle;
    u8     aCorruptionStyle;
    u8     aJungleStyle;
    u8     aSnowStyle;
    u8     aHallowStyle;
    u8     aCrimsonStyle;
    u8     aDesertStyle;
    u8     aOceanStyle;
    s32    aCloudBG;
    s16    aNumClouds;
    f32    aWindSpeed;
    s32    aPlayers;
    s8**   apPlayerNames;
    u8     aSavedAngler;
    s32    aAnglerQuest;
    u8     aSavedStylist;
    u8     aSavedTaxCollector;
    u8     aSavedGolfer;
    s32    aInvasionStartSize;
    s32    aCultistDelay;
    s16    aKillCountLen;
    s32   *apKillCounts;
    u8     aFastForwardTime;
    u8     aKilledFishron;
    u8     aKilledMartians;
    u8     aKilledCultist;
    u8     aKilledMoonLord;
    u8     aKilledHalloweenKing;
    u8     aKilledHalloweenTree;
    u8     aKilledChristmasIceQueen;
    u8     aKilledChristmasSantank;
    u8     aKilledChristmasTree;
    u8     aKilledSolar;
    u8     aKilledVortex;
    u8     aKilledNebula;
    u8     aKilledStardust;
    u8     aActiveSolar;
    u8     aActiveVortex;
    u8     aActiveNebula;
    u8     aActiveStardust;
    u8     aActiveLunarApocalypse;
    u8     aManualParty;
    u8     aInviteParty;
    s32    aPartyCooldown;
    s32    aPartierLen;
    s32   *apPartiers;
    u8     aActiveSandstorm;
    s32    aSandstormTime;
    f32    aSandstormSeverity;
    f32    aSandstormMaxSeverity;
    u8     aSavedBartender;
    u8     aDownedDD2Inv1;
    u8     aDownedDD2Inv2;
    u8     aDownedDD2Inv3;
    u8     aStyle8;
    u8     aStyle9;
    u8     aStyle10;
    u8     aStyle11;
    u8     aStyle12;
    u8     aCombatBookWasUsed;
    s32    aLanternNightCooldown;
    u8     aLanternNight;
    u8     aManualLanternNight;
    u8     aNextLanternWillBeGenuine;
    s32    aTreeTopsLen;
    s32   *apTreeTops;
    u8     aForcedHalloween;
    u8     aForcedChristmas;
    s32    aCopperId;
    s32    aIronId;
    s32    aSilverId;
    s32    aGoldId;
    u8     aBoughtCat;
    u8     aBoughtDog;
    u8     aBoughtBunny;
    u8     aKilledEoL;
    u8     aKilledQueenSlime;
}wld_header_t;