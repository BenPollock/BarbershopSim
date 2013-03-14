#include "fifo.h"
#include <stdio.h>

int main()
{
    fifo f;
    int i;

    for( i = 0; i < 5; i++ )
        Enqueue( &f, i );
    
    for( i = 0; i <= 4; i++ )
        if( Dequeue( &f ) != i )
        {
            printf( "FAIL\n" );
            return 0;
        }
    
    printf( "PASS\n" );

    return 0;
}
