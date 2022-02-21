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

    u32 bits = 0;

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

    u8   *pBuf  = spWld->apFile->apBuf;
    u32  *pPos  = &spWld->apFile->aPos;

    spWld->aHeader.apName = parse_string( pBuf, pPos );
    if ( spWld->aVer >= 179 ) {
        spWld->aHeader.apSeed = parse_string( pBuf, pPos );

        PARSE( pBuf, *pPos, s64, spWld->aHeader.aGeneratorVer );
    }

    if ( spWld->aVer >= 181 ) {
        PARSE_ARRAY( pBuf, *pPos, u8, spWld->aHeader.aGuid, 16 );
    }

    PARSE( pBuf, *pPos, s32,    spWld->aHeader.aId );
    PARSE( pBuf, *pPos, rect_t, spWld->aHeader.aBounds );
    PARSE( pBuf, *pPos, s32,    spWld->aHeader.aHeight );
    PARSE( pBuf, *pPos, s32,    spWld->aHeader.aWidth );

    if ( spWld->aVer >= 209 ) {
        PARSE( pBuf, *pPos, s32,    spWld->aHeader.aGameMode );
    }
    
    if ( spWld->aVer >= 222 ) {
        PARSE( pBuf, *pPos, u8,     spWld->aHeader.aDrunk );
    }

    if ( spWld->aVer >= 227 ) {
        PARSE( pBuf, *pPos, u8,     spWld->aHeader.aFtw );
    }

    if ( spWld->aVer >= 112 && spWld->aVer < 209 ) {
        PARSE( pBuf, *pPos, u8,     spWld->aHeader.aExpert );
    }

    if ( spWld->aVer >= 208 && spWld->aVer < 209 ) {
        PARSE( pBuf, *pPos, u8,     spWld->aHeader.aMaster );
    }

    if ( spWld->aVer >= 141 ) {
        PARSE( pBuf, *pPos, s64,    spWld->aHeader.aCreationTime );
    }

    if ( spWld->aVer >= 63 ) {
        PARSE( pBuf, *pPos, u8,     spWld->aHeader.aMoonType );
    }

    if ( spWld->aVer >= 44 ) {
        PARSE_ARRAY( pBuf, *pPos, s32, spWld->aHeader.aTreeX, 3 );
        PARSE_ARRAY( pBuf, *pPos, s32, spWld->aHeader.aTreeStyles, 4 );
    }

    if ( spWld->aVer >= 60 ) {
        PARSE_ARRAY( pBuf, *pPos, s32, spWld->aHeader.aCaveBackX, 3 );
        PARSE_ARRAY( pBuf, *pPos, s32, spWld->aHeader.aCaveBackStyle, 4 );

        PARSE( pBuf, *pPos, s32, spWld->aHeader.aIceBackStyle );        
    }

    if ( spWld->aVer >= 61 ) {
        PARSE( pBuf, *pPos, s32, spWld->aHeader.aJungleBackStyle );
        PARSE( pBuf, *pPos, s32, spWld->aHeader.aHellBackStyle );
    }

    PARSE( pBuf, *pPos, s32, spWld->aHeader.aSpawnX );
    PARSE( pBuf, *pPos, s32, spWld->aHeader.aSpawnY );
    PARSE( pBuf, *pPos, f64, spWld->aHeader.aGroundLevel );
    PARSE( pBuf, *pPos, f64, spWld->aHeader.aRockLevel );
    PARSE( pBuf, *pPos, f64, spWld->aHeader.aTime );
    PARSE( pBuf, *pPos, u8,  spWld->aHeader.aDay );
    PARSE( pBuf, *pPos, s32, spWld->aHeader.aMoonPhase );
    PARSE( pBuf, *pPos, u8,  spWld->aHeader.aBloodMoon );

    if ( spWld->aVer >= 63 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aIsEclipse );
    }

    PARSE( pBuf, *pPos, s32,  spWld->aHeader.aDungeonX );
    PARSE( pBuf, *pPos, s32,  spWld->aHeader.aDungeonY );

    if ( spWld->aVer >= 56 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aIsCrimson );
    }

    PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledEoc );
    PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledEvilBoss );
    PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledSkeletron );

    if ( spWld->aVer >= 66 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledQueenBee );
    }

    if ( spWld->aVer >= 44 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledDestroyer );
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledTwins );
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledSkeletronPrime );
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledHMBoss );
    }

    if ( spWld->aVer >= 64 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledPlantBoss );
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledGolemBoss );
    }

    if ( spWld->aVer >= 118 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aKilledKingSlime );
    }

    if ( spWld->aVer >= 29 ) {
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aSavedTinkerer );
        PARSE( pBuf, *pPos, u8,  spWld->aHeader.aSavedWizard );
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
    printf( "    Name:         %s\n", sHeader.apName );
    printf( "    Seed:         %s\n", sHeader.apSeed );
    printf( "    GUID:         %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
        sHeader.aGuid[ 0 ], sHeader.aGuid[ 1 ], sHeader.aGuid[ 2 ], sHeader.aGuid[ 3 ],
        sHeader.aGuid[ 4 ], sHeader.aGuid[ 5 ], sHeader.aGuid[ 6 ], sHeader.aGuid[ 7 ],
        sHeader.aGuid[ 8 ], sHeader.aGuid[ 9 ], sHeader.aGuid[ 10 ], sHeader.aGuid[ 11 ],
        sHeader.aGuid[ 12 ], sHeader.aGuid[ 13 ], sHeader.aGuid[ 14 ], sHeader.aGuid[ 15 ] );

    printf( "    ID:           %d\n", sHeader.aId );
    printf( "    Bounds:       %d, %d, %d, %d\n", sHeader.aBounds.x0, sHeader.aBounds.x, sHeader.aBounds.y0, sHeader.aBounds.y );
    printf( "    Height:       %d\n", sHeader.aHeight );
    printf( "    Width:        %d\n", sHeader.aWidth );
    printf( "    Game Mode:    %d\n", sHeader.aGameMode );
    printf( "    Drunk:        %d\n", sHeader.aDrunk );
    printf( "    FTW:          %d\n", sHeader.aFtw );
    printf( "    Expert:       %d\n", sHeader.aExpert );
    printf( "    Master:       %d\n", sHeader.aMaster );
    printf( "    Creation:     %ld\n", sHeader.aCreationTime );
    printf( "    Moon Type:    %d\n", sHeader.aMoonType );
    printf( "    Tree X:       %d, %d, %d\n", sHeader.aTreeX[ 0 ], sHeader.aTreeX[ 1 ], sHeader.aTreeX[ 2 ] );
    printf( "    Tree Style:   %d, %d, %d, %d\n", sHeader.aTreeStyles[ 0 ], sHeader.aTreeStyles[ 1 ], sHeader.aTreeStyles[ 2 ], sHeader.aTreeStyles[ 3 ] );
    printf( "    Cave X:       %d, %d, %d\n", sHeader.aCaveBackX[ 0 ], sHeader.aCaveBackX[ 1 ], sHeader.aCaveBackX[ 2 ] );
    printf( "    Cave Style:   %d, %d, %d, %d\n", sHeader.aCaveBackStyle[ 0 ], sHeader.aCaveBackStyle[ 1 ], sHeader.aCaveBackStyle[ 2 ], sHeader.aCaveBackStyle[ 3 ] );
    printf( "    Ice Style:    %d\n", sHeader.aIceBackStyle );
    printf( "    Jungle Style: %d\n", sHeader.aJungleBackStyle );
    printf( "    Hell Style:   %d\n", sHeader.aHellBackStyle );
    printf( "    Spawn X:      %d\n", sHeader.aSpawnX );
    printf( "    Spawn Y:      %d\n", sHeader.aSpawnY );
    printf( "    Ground Level: %f\n", sHeader.aGroundLevel );
    printf( "    Rock Level:   %f\n", sHeader.aRockLevel );
    printf( "    Time:         %f\n", sHeader.aTime );
    printf( "    Day:          %d\n", sHeader.aDay );
    printf( "    Moon Phase:   %d\n", sHeader.aMoonPhase );
    printf( "    Blood Moon:   %d\n", sHeader.aBloodMoon );
    printf( "    Is Eclipse:   %d\n", sHeader.aIsEclipse );
    printf( "    Dungeon X:    %d\n", sHeader.aDungeonX );
    printf( "    Dungeon Y:    %d\n", sHeader.aDungeonY );
    printf( "    Is Crimson:   %d\n", sHeader.aIsCrimson );
    printf( "    Is Killed Eoc: %d\n", sHeader.aKilledEoc );
    printf( "    Is Killed Evil Boss: %d\n", sHeader.aKilledEvilBoss );
    printf( "    Is Killed Skeletron: %d\n", sHeader.aKilledSkeletron );
    printf( "    Is Killed Queen Bee: %d\n", sHeader.aKilledQueenBee );
    printf( "    Is Killed Destroyer: %d\n", sHeader.aKilledDestroyer );
    printf( "    Is Killed Twins: %d\n", sHeader.aKilledTwins );
    printf( "    Is Killed Skeletron Prime: %d\n", sHeader.aKilledSkeletronPrime );
    printf( "    Is Killed Plantera: %d\n", sHeader.aKilledPlantBoss );
    printf( "    Is Killed Golem: %d\n", sHeader.aKilledGolemBoss );
    printf( "    Is Killed King Slime: %d\n", sHeader.aKilledKingSlime );
    printf( "    Is Saved Tinkerer: %d\n", sHeader.aSavedTinkerer );
    printf( "    Is Saved Wizard: %d\n", sHeader.aSavedWizard );
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
}