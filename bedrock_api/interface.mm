//
//  interface.c
//  workspaces
//
//  Created by Alexandra Beebe on 11/26/22.
//

//
#include "pluginManager.h"

//
#include "interface.h"


//
Class     hasManager ( void )
{
    static void *pluginManager = NULL;

    if (!pluginManager)
        pluginManager = (__bridge void *) NSClassFromString(@"pluginManager");

    return (__bridge Class) pluginManager;
}


//
//
void      plugin ( __uuid__ uuid, const char *name, const char *factory, void *library )
{
    Class pluginManager;
    if (! (pluginManager = hasManager()) )
        return;

    //
    void *symbol;
    if (!(symbol = [pluginManager loadSymbol :library :factory :name ]))
        return;

    //
    uuid_log ( name, uuid );

    [ pluginManager addPlugin :uuid.actual :symbol ];
}
//


//
//
void     *plugin ( __uuid__ uuid )
{
    Class pluginManager;
    if (! (pluginManager = hasManager()) )
        return NULL;

    //
    return [ pluginManager getPlugin : uuid.actual ];
}
//
