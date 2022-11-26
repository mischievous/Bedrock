//
//  uuid.c
//  KEXT
//
//  Created by Alexandra Beebe on 7/8/18.
//  Copyright © 2018 Alexandra Beebe. All rights reserved.
//

//
#include <stdio.h>

//
#include "uuid.h"


//
//
uint128_t  uuid_generate ( void )
{
    __uuid__ uuid = {.actual = 0x0};

    //
    static FILE *urandom = NULL;
    if (urandom == NULL)
    {
        if (!(urandom = (FILE *) fopen ("/dev/urandom", "rb")))
            return uuid.actual;
    }

    //
    if (fread (uuid.bytes,sizeof(__uuid__::bytes), 1, urandom) != 1)
        uuid.actual = 0;

    return uuid.actual;
}
//

//
//
void uuid_log   ( const char *baseName, __uuid__ uuid )
{    
    //
    printf ("%s     %0*llx-%0*llx\n", baseName, 16, uuid.d064.u0, 16, uuid.d064.u1);
}
//


//
//
void uuid_dump ( const char *baseName, __uuid__ uuid )
{
    printf ("%s", baseName);
    for (uint64_t idx = 0; idx < sizeof (__uuid__::bytes); idx += 1)
        printf (" : %02x", uuid.bytes[idx]);
    printf ("\n");
}
//
//
