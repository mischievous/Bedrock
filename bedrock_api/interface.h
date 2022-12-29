//
//  interface.h
//  workspaces
//
//  Created by Alexandra Beebe on 11/26/22.
//

#ifndef INTERFACE
#define INTERFACE

#include "bedrock_uuid.h"

//
extern void  plugin ( bedrock_uuid uuid, const char *name, const char *factory, void *library );
extern void *plugin ( bedrock_uuid uuid );

#endif /* interface_h */
