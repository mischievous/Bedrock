//
//  interface.h
//  workspaces
//
//  Created by Alexandra Beebe on 11/26/22.
//

#ifndef INTERFACE
#define INTERFACE

#include "uuid.h"

//
extern void  plugin ( __uuid__ uuid, const char *name, const char *factory, void *library );
extern void *plugin ( __uuid__ uuid );



#endif /* interface_h */
