#include "helpers.h"

#include <stdio.h>

int main()
{
    if( GetCashierDelay( "cash" ) == 1*60 && GetCashierDelay( "credit" ) == 2*60 )
        printf( "PASS\n" );
    else
        printf( "FAIL\n" );

    return 0;
}
