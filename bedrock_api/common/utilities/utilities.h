//
//  utilities.h
//  plugin
//
//  Created by Alexandra Beebe on 11/22/22.
//

#include <stdint.h>
#include <sys/stat.h>

#ifndef utilities_h
#define utilities_h

extern uint64_t    bedrock_exists     ( const char *ipath, uint64_t length = 1);
extern uint64_t    bedrock_exists     ( const FILE *file , uint64_t length = 1);

extern uint32_t    bedrock_directory  ( const char *ipath );
extern void        bedrock_remove     ( const char *ipath, uint64_t directory = 1 );
extern void        bedrock_copy       ( const char *s    , const char *d, uint64_t replace = true );
extern uint64_t    bedrock_makedir    ( const char *ipath, mode_t mode = S_IRWXU, const char *sep = 0x0 );
extern const char *bedrock_realpath   (       char *ipath, uint32_t count );

#endif /* utilities_h */
