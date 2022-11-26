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

extern uint32_t bedrock_exists    ( const char *ipath );
extern uint32_t bedrock_directory ( const char *ipath );
extern void     bedrock_remove    ( const char *ipath );
extern void     bedrock_copy      ( const char *s    , const char *d, uint64_t replace = true );
extern uint64_t bedrock_makedir   ( const char *ipath, mode_t mode = S_IRWXU );

#endif /* utilities_h */
