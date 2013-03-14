#include "helpers.h"


#include <stdio.h>
#include <string.h>

typedef struct test
{
    int sec;
    char* timestamp;
} test;

int main()
{
    test sec2str[] = { { 0, "00:00:00" }, { 1, "00:00:01" }, { 59, "00:00:59" },
        { 60, "00:01:00" }, { 60*60 - 1, "00:59:59" }, { 60*60, "01:00:00" },
        { 24*60*60 - 1, "23:59:59" } };

    for( int i = 0; i < 7; i++ )
    {
        int sec = TimeStampStr2Sec( sec2str[i].timestamp );
        if( sec != sec2str[i].sec )
        {
            printf( "FAIL\n" );
            return 0;
        }
    }

    printf( "PASS\n" );
    return 0;
}
