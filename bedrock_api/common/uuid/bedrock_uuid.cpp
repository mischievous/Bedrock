//
//  uuid.c
//  KEXT
//
//  Created by Alexandra Beebe on 7/8/18.
//  Copyright © 2018 Alexandra Beebe. All rights reserved.
//

//
#include <stdio.h>
#include <memory.h>

//
#include "bedrock_uuid.h"


//
//
uint128_t  uuid_generate ( void )
{
    uint128_t uuid = 0x0;

    //
    static FILE *urandom = NULL;
    if (urandom == NULL)
    {
        if (!(urandom = (FILE *) fopen ("/dev/urandom", "rb")))
            return uuid;
    }

    //
    if (fread (&uuid, sizeof(uuid), 1, urandom) != 1)
        uuid = 0;

    return uuid;
}
//


//
//
void       uuid_generate ( bedrock_uuid &uuid )
{
    uuid.actual = uuid_generate();

    uuid_format ( uuid );
}
//


//
//
void       uuid_log      ( const char *baseName, bedrock_uuid uuid )
{    
    //
    printf ("%s     %0*llx-%0*llx\n", baseName, 16, uuid.d064.u0, 16, uuid.d064.u1);
}
//


//
//
void       uuid_dump     ( const char *baseName, bedrock_uuid uuid )
{
    printf ("%s", baseName);
    for (uint64_t idx = 0; idx < sizeof (bedrock_uuid::bytes); idx += 1)
        printf (" : %02x", uuid.bytes[idx]);
    printf ("\n");
}
//
//


//
//
void       uuid_format   ( bedrock_uuid &uuid, uint32_t size )
{
    memset   (uuid.format, 0x0, sizeof (uuid.format));
    snprintf (uuid.format, sizeof (uuid.format), "%08x-%04x-%04x-%04x-%08x%04x", uuid.i.u0, uuid.i.u1, uuid.i.u2, uuid.i.u3, uuid.i.u4, uuid.i.u5);

    //
    uuid.format[(size) ? size:strlen(uuid.format)] = '\0';
}
//
