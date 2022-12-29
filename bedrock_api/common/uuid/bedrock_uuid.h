//
//  uuid.h
//  KEXT
//
//  Created by Alexandra Beebe on 7/8/18.
//  Copyright © 2018 Alexandra Beebe. All rights reserved.
//

#ifndef BEDROCK_UUID
#define BEDROCK_UUID

typedef unsigned char     uint08_t;
typedef unsigned __int128 uint128_t;

//
#include <stdint.h>

#pragma pack(push, 1)
typedef struct _bedrock_uuid_
{
    union
    {
        uint128_t actual;

        //
        struct
        {
            uint08_t bytes[16];
        };

        struct
        {
            uint16_t u2;
            uint16_t u1;
            uint32_t u0;

            uint16_t u5;
            uint32_t u4;
            uint16_t u3;

        } i;

        struct
        {
            uint64_t u2:16;
            uint64_t u1:16;
            uint64_t u0:32;
            uint64_t u4:48;
            uint64_t u3:16;
        } data;

        struct
        {
            uint64_t u0;
            uint64_t u1;
        } d064;
    };

    //
    char format[72]; // 00000000-0000-0000-0000-000000000000

} bedrock_uuid;
#pragma pack(pop)

//
uint128_t  uuid_generate ( void );
void       uuid_generate ( bedrock_uuid &uuid );

void       uuid_log      ( const char *baseName, bedrock_uuid uuid );
void       uuid_dump     ( const char *baseName, bedrock_uuid uuid );

//
void       uuid_format   ( bedrock_uuid &uuid, uint32_t size = 0 );


#endif /* uuid_h */
