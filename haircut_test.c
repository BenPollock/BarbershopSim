#include "helpers.h"

#include <stdio.h>
#include <math.h>

int main()
{
    if( fabs( GetHairCutPrice( "man" ) - 20.00 ) < 0.001 &&
        fabs( GetHairCutPrice( "woman" ) - 30.00 ) < 0.001 &&
        fabs( GetHairCutPrice( "child" ) - 10.00 ) < 0.001 )
        printf( "PASS\n" );
    else
        printf( "FAIL\n" );

    return 0;
}
