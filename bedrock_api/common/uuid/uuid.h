//
//  uuid.h
//  KEXT
//
//  Created by Alexandra Beebe on 7/8/18.
//  Copyright © 2018 Alexandra Beebe. All rights reserved.
//

#ifndef UUID
#define UUID

typedef unsigned char     uint08_t;
typedef unsigned __int128 uint128_t;

//
#include <stdint.h>

#pragma pack(push, 1)
typedef union __uuid__
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

} __uuid__;
#pragma pack(pop)

//
uint128_t  uuid_generate ( void );
void       uuid_log      ( const char *baseName, __uuid__ uuid );
void       uuid_dump     ( const char *baseName, __uuid__ uuid ); 

#endif /* uuid_h */
