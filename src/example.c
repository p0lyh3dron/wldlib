/*
 *    example.c    --    example of using the wldlib library
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 21, 2022
 * 
 *    An example of using the wldlib library.
 */
#include "wldlib.h"

#include <stdio.h>

/*
 *    Entry.
 *
 *    @return int
 *        0 on success, -1 on failure.
 */
int main( void ) {
    wld_t *pWld = wld_open( "world.wld" );
    if( pWld == NULL ) {
        printf( "Failed to open world.wld\n" );
        return -1;
    }

    wld_write( pWld, "world_modified.wld" );

    wld_free( pWld );

    return 0;
}