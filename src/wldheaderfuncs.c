/*
 *    wldheaderfuncs.c    --    Source file for the WLD file header
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 * 
 *    Source file for parsing the WLD file header.
 */
#include "wldheaderfuncs.h"
#include "wldheader.h"
#include "parseutil.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
/*
 *    Peeks at the world header and returns the version of the world.
 *    Returns -1 if the world is invalid.
 *
 *    @param wld_t* 
 *        The world to check.
 * 
 *    @return s32 
 *        The version of the world.
 */
s32 wld_get_version( wld_t *spWld ) {
    if( !spWld ) {
        fprintf( stderr, "wld_get_version( wld_t* ): World is NULL.\n" );
        return -1;
    }

    if ( !spWld->apFile ) {
        fprintf( stderr, "wld_get_version( wld_t* ): World file stream is NULL.\n" );
        return -1;
    }

    if( !spWld->apFile->apBuf ) {
        fprintf( stderr, "wld_get_version( wld_t* ): World file buffer is NULL.\n" );
        return -1;
    }

    return spWld->apFile->apBuf[ 0 ];
}
/*
 *    Parses the world info header.
 *
 *    @param wld_t*
 *        The world to parse.
 * 
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_info_parse( wld_t *spWld ) {
    if( !spWld ) {
        fprintf( stderr, "wld_header_parse( wld_t* ): World is NULL.\n" );
        return 0;
    }
    u8   *pBuf  = spWld->apFile->apBuf;
    u32  *pPos  = &spWld->apFile->aPos;

    PARSE( pBuf, *pPos, u32, spWld->aInfo.aVer );

    PARSE_ARRAY( pBuf, *pPos, u8, spWld->aInfo.aSig, 7 );

    PARSE( pBuf, *pPos, s8,  spWld->aInfo.aWorldType );
    PARSE( pBuf, *pPos, s32, spWld->aInfo.aRevisions );
    PARSE( pBuf, *pPos, s64, spWld->aInfo.aFavorite );
    PARSE( pBuf, *pPos, s16, spWld->aInfo.aNumSections );
    
    spWld->aInfo.apSections = ( s32* )malloc( sizeof( s32 ) * spWld->aInfo.aNumSections );
    if( !spWld->aInfo.apSections ) {
        fprintf( stderr, "wld_header_parse( wld_t* ): Failed to allocate memory for sections.\n" );
        return 0;
    }
    
    PARSE_ARRAY( pBuf, *pPos, s32, spWld->aInfo.apSections, spWld->aInfo.aNumSections );

    PARSE( pBuf, *pPos, s16, spWld->aInfo.aTileMask );

    s32 bits = 0;

    if ( spWld->aInfo.aTileMask % 8 == 0 )
        bits = spWld->aInfo.aTileMask / 8;
    else
        bits = spWld->aInfo.aTileMask / 8 + 1;

    spWld->aInfo.apUVs = ( s8* )malloc( sizeof( s8 ) * bits );
    if( !spWld->aInfo.apUVs ) {
        fprintf( stderr, "wld_header_parse( wld_t* ): Failed to allocate memory for UVs.\n" );
        return 0;
    }

    PARSE_ARRAY( pBuf, *pPos, s8, spWld->aInfo.apUVs, bits );
#if DEBUG
    wld_info_header_dump( spWld->aInfo );
#endif /* DEBUG  */

    return 1;
}
/*
 *    Parses the world format header.
 *
 *    @param wld_t*
 *        The world to parse.
 * 
 *    @return u32
 *        1 on success, 0 on failure.
 */
u32 wld_header_parse( wld_t *spWld ) {
    if ( !wld_info_parse( spWld ) ) {
        fprintf( stderr, "wld_header_parse( wld_t* ): Failed to parse world info header.\n" );
        return 0;
    }

    u8           *pBuf  = spWld->apFile->apBuf;
    u32          *pPos  = &spWld->apFile->aPos;
    wld_header_t *pHeader = &spWld->aHeader;

    pHeader->apName = parse_string( pBuf, pPos );
    if ( spWld->aVer >= 179 ) {
        pHeader->apSeed = parse_string( pBuf, pPos );

        PARSE( pBuf, *pPos, s64, pHeader->aGeneratorVer );
    }

    if ( spWld->aVer >= 181 ) {
        PARSE_ARRAY( pBuf, *pPos, u8, pHeader->aGuid, 16 );
    }

    PARSE( pBuf, *pPos, s32,    pHeader->aId );
    PARSE( pBuf, *pPos, rect_t, pHeader->aBounds );
    PARSE( pBuf, *pPos, s32,    pHeader->aHeight );
    PARSE( pBuf, *pPos, s32,    pHeader->aWidth );

    if ( spWld->aVer >= 209 ) {
        PARSE( pBuf, *pPos, s32,    pHeader->aGameMode );
    }

    /* Figure this out later.  */
    *pPos += 3;
    
    if ( spWld->aVer >= 222 ) {
        PARSE( pBuf, *pPos, u8,     pHeader->aDrunk );
    }

    if ( spWld->aVer >= 227 ) {
        PARSE( pBuf, *pPos, u8,     pHeader->aFtw );
    }

    if ( spWld->aVer >= 112 && spWld->aVer < 209 ) {
        PARSE( pBuf, *pPos, u8,     pHeader->aExpert );
    }

    if ( spWld->aVer >= 208 && spWld->aVer < 209 ) {
        PARSE( pBuf, *pPos, u8,     pHeader->aMaster );
    }

    if ( spWld->aVer >= 141 ) {
        PARSE( pBuf, *pPos, s64,    pHeader->aCreationTime );
    }

    if ( spWld->aVer >= 63 ) {
        PARSE( pBuf, *pPos, u8,     pHeader->aMoonType );
    }

    if ( spWld->aVer >= 44 ) {
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->aTreeX, 3 );
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->aTreeStyles, 4 );
    }

    if ( spWld->aVer >= 60 ) {
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->aCaveBackX, 3 );
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->aCaveBackStyle, 4 );

        PARSE( pBuf, *pPos, s32, pHeader->aIceBackStyle );        
    }

    if ( spWld->aVer >= 61 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aJungleBackStyle );
        PARSE( pBuf, *pPos, s32, pHeader->aHellBackStyle );
    }

    PARSE( pBuf, *pPos, s32, pHeader->aSpawnX );
    PARSE( pBuf, *pPos, s32, pHeader->aSpawnY );
    PARSE( pBuf, *pPos, f64, pHeader->aGroundLevel );
    PARSE( pBuf, *pPos, f64, pHeader->aRockLevel );
    PARSE( pBuf, *pPos, f64, pHeader->aTime );
    PARSE( pBuf, *pPos, u8,  pHeader->aDay );
    PARSE( pBuf, *pPos, s32, pHeader->aMoonPhase );
    PARSE( pBuf, *pPos, u8,  pHeader->aBloodMoon );

    if ( spWld->aVer >= 63 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aIsEclipse );
    }

    PARSE( pBuf, *pPos, s32,  pHeader->aDungeonX );
    PARSE( pBuf, *pPos, s32,  pHeader->aDungeonY );

    if ( spWld->aVer >= 56 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aIsCrimson );
    }

    PARSE( pBuf, *pPos, u8,  pHeader->aKilledEoc );
    PARSE( pBuf, *pPos, u8,  pHeader->aKilledEvilBoss );
    PARSE( pBuf, *pPos, u8,  pHeader->aKilledSkeletron );

    if ( spWld->aVer >= 66 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledQueenBee );
    }

    if ( spWld->aVer >= 44 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledDestroyer );
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledTwins );
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledSkeletronPrime );
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledHMBoss );
    }

    if ( spWld->aVer >= 64 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledPlantBoss );
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledGolemBoss );
    }

    if ( spWld->aVer >= 118 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledKingSlime );
    }

    if ( spWld->aVer >= 29 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aSavedTinkerer );
        PARSE( pBuf, *pPos, u8,  pHeader->aSavedWizard );
    }

    if ( spWld->aVer >= 34 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aSavedMechanic );
    }

    if ( spWld->aVer >= 29 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aDefeatedGoblins );
    }

    if ( spWld->aVer >= 32 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledClown );
    }

    if ( spWld->aVer >= 37 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledFrost );
    }

    if ( spWld->aVer >= 56 ) {
        PARSE( pBuf, *pPos, u8,  pHeader->aKilledPirates );
    }

    PARSE( pBuf, *pPos, u8,  pHeader->aBrokeOrb );
    PARSE( pBuf, *pPos, u8,  pHeader->aMeteor );
    PARSE( pBuf, *pPos, u8,  pHeader->aShadowOrbSmashed );

    if ( spWld->aVer >= 23 ) {
        PARSE( pBuf, *pPos, s32,  pHeader->aAltarCount );
        PARSE( pBuf, *pPos, u8,   pHeader->aHardMode );
    }

    PARSE( pBuf, *pPos, s32,   pHeader->aInvasionDelay );
    PARSE( pBuf, *pPos, s32,   pHeader->aInvasionSize );
    PARSE( pBuf, *pPos, s32,   pHeader->aInvasionType );
    PARSE( pBuf, *pPos, f64,   pHeader->aInvasionX );

    if ( spWld->aVer >= 118 ) {
        PARSE( pBuf, *pPos, f64,   pHeader->aSlimeRainTime );
    }

    if ( spWld->aVer >= 113 ) {
        PARSE( pBuf, *pPos, u8,   pHeader->aSundialCooldown );
    }

    if ( spWld->aVer >= 53 ) {
        PARSE( pBuf, *pPos, u8,    pHeader->aIsRaining );
        PARSE( pBuf, *pPos, s32,   pHeader->aRainTime );
        PARSE( pBuf, *pPos, f32,   pHeader->aMaxRain );
    }

    if ( spWld->aVer >= 54 ) {
        PARSE( pBuf, *pPos, s32,   pHeader->aOreTier1 );
        PARSE( pBuf, *pPos, s32,   pHeader->aOreTier2 );
        PARSE( pBuf, *pPos, s32,   pHeader->aOreTier3 );
    }

    if ( spWld->aVer >= 55 ) {
        PARSE( pBuf, *pPos, u8,   pHeader->aTreeStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aCorruptionStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aJungleStyle );
    }

    if ( spWld->aVer >= 60 ) {
        PARSE( pBuf, *pPos, u8,   pHeader->aSnowStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aHallowStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aCrimsonStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aDesertStyle );
        PARSE( pBuf, *pPos, u8,   pHeader->aOceanStyle );
        PARSE( pBuf, *pPos, s32,  pHeader->aCloudBG );
    }

    if ( spWld->aVer >= 62 ) {
        PARSE( pBuf, *pPos, s16,   pHeader->aNumClouds );
        PARSE( pBuf, *pPos, f32,   pHeader->aWindSpeed );
    }

    if ( spWld->aVer >= 95 ) {
        PARSE( pBuf, *pPos, s32,   pHeader->aPlayers );
        
        pHeader->apPlayerNames = ( s8** )malloc( sizeof( char* ) * pHeader->aPlayers );
        for ( int i = 0; i < pHeader->aPlayers; i++ ) {
            pHeader->apPlayerNames[ i ] = parse_string( pBuf, pPos );
        }
    }

    if ( spWld->aVer >= 99 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aSavedAngler );
    }

    if ( spWld->aVer >= 101 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aAnglerQuest );
    }

    if ( spWld->aVer >= 104 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aSavedStylist );
    }

    if ( spWld->aVer >= 129 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aSavedTaxCollector );
    }

    if ( spWld->aVer >= 201 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aSavedGolfer );
    }

    if ( spWld->aVer >= 107 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aInvasionStartSize );
    }

    if ( spWld->aVer >= 108 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aCultistDelay );
    }

    if ( spWld->aVer >= 109 ) {
        PARSE( pBuf, *pPos, s16, pHeader->aKillCountLen );

        pHeader->apKillCounts = ( s32* )malloc( sizeof( s32 ) * pHeader->aKillCountLen );
        if ( !pHeader->apKillCounts ) {
            fprintf( stderr, "wld_header_parse( wld_t* ): Failed to allocate memory for kill counts\n" );
            return 0;
        }

        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->apKillCounts, pHeader->aKillCountLen );
    }

    if ( spWld->aVer >= 128 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aFastForwardTime );
    }

    if ( spWld->aVer >= 131 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aKilledFishron );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledMartians );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledCultist );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledMoonLord );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledHalloweenKing );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledHalloweenTree );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledChristmasIceQueen );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledChristmasSantank );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledChristmasTree );
    }

    if ( spWld->aVer >= 140 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aKilledSolar );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledVortex );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledNebula );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledStardust );
        PARSE( pBuf, *pPos, u8, pHeader->aActiveSolar );
        PARSE( pBuf, *pPos, u8, pHeader->aActiveVortex );
        PARSE( pBuf, *pPos, u8, pHeader->aActiveNebula );
        PARSE( pBuf, *pPos, u8, pHeader->aActiveStardust );
        PARSE( pBuf, *pPos, u8, pHeader->aActiveLunarApocalypse );
    }

    if ( spWld->aVer >= 170 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aManualParty );
        PARSE( pBuf, *pPos, u8, pHeader->aInviteParty );
        PARSE( pBuf, *pPos, s32, pHeader->aPartyCooldown );
        PARSE( pBuf, *pPos, s32, pHeader->aPartierLen );

        pHeader->apPartiers = ( s32* )malloc( sizeof( s32 ) * pHeader->aPartierLen );
        if ( !pHeader->apPartiers ) {
            fprintf( stderr, "wld_header_parse( wld_t* ): Failed to allocate memory for partiers\n" );
            return 0;
        }
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->apPartiers, pHeader->aPartierLen );
    }

    if ( spWld->aVer >= 174 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aActiveSandstorm );
        PARSE( pBuf, *pPos, s32, pHeader->aSandstormTime );
        PARSE( pBuf, *pPos, f32, pHeader->aSandstormSeverity );
        PARSE( pBuf, *pPos, f32, pHeader->aSandstormMaxSeverity );
    }

    if ( spWld->aVer >= 178 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aSavedBartender );
        PARSE( pBuf, *pPos, u8, pHeader->aDownedDD2Inv1 );
        PARSE( pBuf, *pPos, u8, pHeader->aDownedDD2Inv2 );
        PARSE( pBuf, *pPos, u8, pHeader->aDownedDD2Inv3 );
    }

    if ( spWld->aVer >= 195 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aStyle8 );
    }

    if ( spWld->aVer >= 215 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aStyle9 );
    }

    if ( spWld->aVer >= 196 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aStyle10 );
        PARSE( pBuf, *pPos, u8, pHeader->aStyle11 );
        PARSE( pBuf, *pPos, u8, pHeader->aStyle12 );
    }

    if ( spWld->aVer >= 204 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aCombatBookWasUsed );
    }

    if ( spWld->aVer >= 207 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aLanternNightCooldown );
        PARSE( pBuf, *pPos, u8,  pHeader->aLanternNight );
        PARSE( pBuf, *pPos, u8,  pHeader->aManualLanternNight );
        PARSE( pBuf, *pPos, u8,  pHeader->aNextLanternWillBeGenuine );
    }

    if ( spWld->aVer >= 211 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aTreeTopsLen );

        pHeader->apTreeTops = ( s32* )malloc( sizeof( s32 ) * pHeader->aTreeTopsLen );
        if ( !pHeader->apTreeTops ) {
            fprintf( stderr, "wld_header_parse( wld_t* ): Failed to allocate memory for tree tops\n" );
            return 0;
        }
        PARSE_ARRAY( pBuf, *pPos, s32, pHeader->apTreeTops, pHeader->aTreeTopsLen );
    }

    if ( spWld->aVer >= 212 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aForcedHalloween );
        PARSE( pBuf, *pPos, u8, pHeader->aForcedChristmas );
    }

    if ( spWld->aVer >= 216 ) {
        PARSE( pBuf, *pPos, s32, pHeader->aCopperId );
        PARSE( pBuf, *pPos, s32, pHeader->aIronId );
        PARSE( pBuf, *pPos, s32, pHeader->aSilverId );
        PARSE( pBuf, *pPos, s32, pHeader->aGoldId );
    }

    if ( spWld->aVer >= 217 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aBoughtCat );
        PARSE( pBuf, *pPos, u8, pHeader->aBoughtDog );
        PARSE( pBuf, *pPos, u8, pHeader->aBoughtBunny );
    }

    if ( spWld->aVer >= 223 ) {
        PARSE( pBuf, *pPos, u8, pHeader->aKilledEoL );
        PARSE( pBuf, *pPos, u8, pHeader->aKilledQueenSlime );
    }
#if DEBUG
    wld_header_dump( spWld->aHeader );
#endif /* DEBUG  */
    return 1;
}
/*
 *    Dumps the contents of the world format header to stdout.
 *
 *    @param wld_header_t 
 *        The header to dump.
 */
void wld_header_dump( wld_header_t sHeader ) {
    printf( "World Header:\n" );
    printf( "    Name:                   %s\n", sHeader.apName );
    printf( "    Seed:                   %s\n", sHeader.apSeed );
    printf( "    GUID:                   %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
        sHeader.aGuid[ 0 ], sHeader.aGuid[ 1 ], sHeader.aGuid[ 2 ], sHeader.aGuid[ 3 ],
        sHeader.aGuid[ 4 ], sHeader.aGuid[ 5 ], sHeader.aGuid[ 6 ], sHeader.aGuid[ 7 ],
        sHeader.aGuid[ 8 ], sHeader.aGuid[ 9 ], sHeader.aGuid[ 10 ], sHeader.aGuid[ 11 ],
        sHeader.aGuid[ 12 ], sHeader.aGuid[ 13 ], sHeader.aGuid[ 14 ], sHeader.aGuid[ 15 ] );

    printf( "    ID:                     %d\n", sHeader.aId );
    printf( "    Bounds:                 %d, %d, %d, %d\n", sHeader.aBounds.x0, sHeader.aBounds.x, sHeader.aBounds.y0, sHeader.aBounds.y );
    printf( "    Height:                 %d\n", sHeader.aHeight );
    printf( "    Width:                  %d\n", sHeader.aWidth );
    printf( "    Game Mode:              %d\n", sHeader.aGameMode );
    printf( "    Drunk:                  %d\n", sHeader.aDrunk );
    printf( "    FTW:                    %d\n", sHeader.aFtw );
    printf( "    Expert:                 %d\n", sHeader.aExpert );
    printf( "    Master:                 %d\n", sHeader.aMaster );
    printf( "    Creation:               %ld\n", sHeader.aCreationTime );
    printf( "    Moon Type:              %d\n", sHeader.aMoonType );
    printf( "    Tree X:                 %d, %d, %d\n", sHeader.aTreeX[ 0 ], sHeader.aTreeX[ 1 ], sHeader.aTreeX[ 2 ] );
    printf( "    Tree Style:             %d, %d, %d, %d\n", sHeader.aTreeStyles[ 0 ], sHeader.aTreeStyles[ 1 ], sHeader.aTreeStyles[ 2 ], sHeader.aTreeStyles[ 3 ] );
    printf( "    Cave X:                 %d, %d, %d\n", sHeader.aCaveBackX[ 0 ], sHeader.aCaveBackX[ 1 ], sHeader.aCaveBackX[ 2 ] );
    printf( "    Cave Style:             %d, %d, %d, %d\n", sHeader.aCaveBackStyle[ 0 ], sHeader.aCaveBackStyle[ 1 ], sHeader.aCaveBackStyle[ 2 ], sHeader.aCaveBackStyle[ 3 ] );
    printf( "    Ice Style:              %d\n", sHeader.aIceBackStyle );
    printf( "    Jungle Style:           %d\n", sHeader.aJungleBackStyle );
    printf( "    Hell Style:             %d\n", sHeader.aHellBackStyle );
    printf( "    Spawn X:                %d\n", sHeader.aSpawnX );
    printf( "    Spawn Y:                %d\n", sHeader.aSpawnY );
    printf( "    Ground Level:           %f\n", sHeader.aGroundLevel );
    printf( "    Rock Level:             %f\n", sHeader.aRockLevel );
    printf( "    Time:                   %f\n", sHeader.aTime );
    printf( "    Day:                    %d\n", sHeader.aDay );
    printf( "    Moon Phase:             %d\n", sHeader.aMoonPhase );
    printf( "    Blood Moon:             %d\n", sHeader.aBloodMoon );
    printf( "    Eclipse:                %d\n", sHeader.aIsEclipse );
    printf( "    Dungeon X:              %d\n", sHeader.aDungeonX );
    printf( "    Dungeon Y:              %d\n", sHeader.aDungeonY );
    printf( "    Crimson:                %d\n", sHeader.aIsCrimson );
    printf( "    Killed Eoc:             %d\n", sHeader.aKilledEoc );
    printf( "    Killed Evil Boss:       %d\n", sHeader.aKilledEvilBoss );
    printf( "    Killed Skeletron:       %d\n", sHeader.aKilledSkeletron );
    printf( "    Killed Queen Bee:       %d\n", sHeader.aKilledQueenBee );
    printf( "    Killed Destroyer:       %d\n", sHeader.aKilledDestroyer );
    printf( "    Killed Twins:           %d\n", sHeader.aKilledTwins );
    printf( "    Killed Skeletron Prime: %d\n", sHeader.aKilledSkeletronPrime );
    printf( "    Killed Plantera:        %d\n", sHeader.aKilledPlantBoss );
    printf( "    Killed Golem:           %d\n", sHeader.aKilledGolemBoss );
    printf( "    Killed King Slime:      %d\n", sHeader.aKilledKingSlime );
    printf( "    Saved Tinkerer:         %d\n", sHeader.aSavedTinkerer );
    printf( "    Saved Wizard:           %d\n", sHeader.aSavedWizard );
    printf( "    Saved Mechanic:         %d\n", sHeader.aSavedMechanic );
    printf( "    Defeated Goblins:       %d\n", sHeader.aDefeatedGoblins );
    printf( "    Killed Clown:           %d\n", sHeader.aKilledClown );
    printf( "    Killed Pirates:         %d\n", sHeader.aKilledPirates );
    printf( "    Broke Orb:              %d\n", sHeader.aBrokeOrb );
    printf( "    Meteor Fell:            %d\n", sHeader.aMeteor );
    printf( "    Smashed Orbs:           %d\n", sHeader.aShadowOrbSmashed );
    printf( "    Altars Destroyed:       %d\n", sHeader.aAltarCount );
    printf( "    Hardmode:               %d\n", sHeader.aHardMode );
    printf( "    Invasion Delay:         %d\n", sHeader.aInvasionDelay );
    printf( "    Invasion Size:          %d\n", sHeader.aInvasionSize );
    printf( "    Invasion Type:          %d\n", sHeader.aInvasionType );
    printf( "    Invasion X:             %f\n", sHeader.aInvasionX );
    printf( "    Slime Rain Time:        %f\n", sHeader.aSlimeRainTime );
    printf( "    Sundial Cooldown:       %d\n", sHeader.aSundialCooldown );
    printf( "    Raining:                %d\n", sHeader.aIsRaining );
    printf( "    Rain Time:              %d\n", sHeader.aRainTime );
    printf( "    Max Rain:               %f\n", sHeader.aMaxRain );
    printf( "    Tier 1 Ore:             %d\n", sHeader.aOreTier1 );
    printf( "    Tier 2 Ore:             %d\n", sHeader.aOreTier2 );
    printf( "    Tier 3 Ore:             %d\n", sHeader.aOreTier3 );
    printf( "    Tree Style:             %d\n", sHeader.aTreeStyle );
    printf( "    Corruption Style:       %d\n", sHeader.aCorruptionStyle );
}
/*
 *    Dumps the contents of the world info header to stdout.
 *
 *    @param wld_info_header_t 
 *        The header to dump.
 */
void wld_info_header_dump( wld_info_header_t sInfo ) {
    printf( "World Info Header:\n" );
    printf( "    Version:      %d\n",  sInfo.aVer );
    printf( "    Signature:    %s\n",  sInfo.aSig );
    printf( "    World Type:   %d\n",  sInfo.aWorldType );
    printf( "    Revisions:    %d\n",  sInfo.aRevisions );
    printf( "    Favorite:     %ld\n", sInfo.aFavorite );
    printf( "    Num Sections: %d\n",  sInfo.aNumSections );
    printf( "    Section Offsets:\n" );
    for( s16 i = 0; i < sInfo.aNumSections; i++ ) {
        printf( "        %d: %d\n", i, sInfo.apSections[ i ] );
    }
    printf( "\n" );
    printf( "    Tile Mask: %d\n", sInfo.aTileMask );
    printf( "    UVs:\n" );
    for( s16 i = 0; i < sInfo.aTileMask / 8 + 1; i++ ) {
        printf( "        %d: %d\n", i, sInfo.apUVs[ i ] );
    }
    printf( "\n" );
}
/*
 *    Frees a world info header.
 *
 *    @param wld_info_header_t 
 *        The world header to free.
 */
void wld_info_header_free( wld_info_header_t sHeader ) {
    if( sHeader.apSections )
        free( sHeader.apSections );
    if( sHeader.apUVs )
        free( sHeader.apUVs );
}
/*
 *    Frees a world format header.
 *
 *    @param wld_header_t 
 *        The world header to free.
 */
void wld_header_free( wld_header_t sHeader ) {
    if( sHeader.apName )
        free( sHeader.apName );
    if( sHeader.apSeed )
        free( sHeader.apSeed );
    if ( sHeader.apPlayerNames ) {
        for ( s32 i = 0; i < sHeader.aPlayers; i++ ) {
            if ( sHeader.apPlayerNames[ i ] )
                free( sHeader.apPlayerNames[ i ] );
        }
        free( sHeader.apPlayerNames );
    }
    if ( sHeader.apKillCounts ) {
        free( sHeader.apKillCounts );
    }
    if ( sHeader.apPartiers ) {
        free( sHeader.apPartiers );
    }
    if ( sHeader.apTreeTops ) {
        free( sHeader.apTreeTops );
    }
}