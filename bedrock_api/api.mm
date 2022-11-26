//
//  plugin.m
//  plugin
//
//  Created by Alexandra Beebe on 7/30/22.
//

//
#include "api.h"
#include "interface.h"

//
#define FACTORY         ("_ZN%lu%s7factoryEPKc")

//
//
bedrockPlugin:: bedrockPlugin ( NSBundle *bundle, bedrock *owner, void *library ) : bundle(bundle), owner ( owner ), library (library)
{
}
//


//
//
bedrockPlugin::~bedrockPlugin ( void )
{
}
//


#pragma mark private



#pragma mark public
//
//
void      bedrockPlugin::plugin           ( __uuid__ uuid, const char *name )
{
    ::plugin (uuid, name, FACTORY, library);
}
//


//
//
void     *bedrockPlugin::plugin           ( __uuid__ uuid )
{
    return ::plugin ( uuid );
}
//



//
//
id        bedrockPlugin::addView          ( const char *nibName )
{
    return [owner addView :nibName :bundle];
}
//


//
//
void      bedrockPlugin::addMenu          ( callback_plugin *callback, const std::vector <std::string> path )
{
    //
    NSMenu *menu = [owner accessMenu];

    //
    uint64_t count = path.size();

    //
    for (uint64_t idx = 0; idx < count; idx += 1)
    {
        //
        NSString *title = [NSString stringWithUTF8String:path[idx].c_str()];

        //
        NSMenuItem *item;
        if (!(item = [menu itemWithTitle:title]))
        {
            item = [[NSMenuItem alloc] initWithTitle:title action:NULL keyEquivalent:@""];

            //
            [menu addItem:item];

            if ( (idx + 1) < count )
                item.submenu = [[NSMenu alloc] initWithTitle:title];
            else
            {
                item.enabled = 1;
                item.target  = owner;
                item.action  = @selector (actionMenu:);
                item.tag     = (long) callback;
            }
        }

        //
        menu = item.submenu;
    }
}
//


//
//
void      bedrockPlugin::performMenu      ( const std::vector <std::string> path )
{
    //
    NSMenu *menu = [owner accessMenu];

    //
    uint64_t count = path.size();

    //
    for (uint64_t idx = 0; idx < count; idx += 1)
    {
        //
        NSString *title = [NSString stringWithUTF8String:path[idx].c_str()];

        NSMenuItem *item;
        if (!(item = [menu itemWithTitle:title]))
            return;

        //
        if ( (idx + 1) < count )
            menu = item.submenu;
        else
            [menu performActionForItemAtIndex:[menu indexOfItemWithTitle:title]];
    }
}
//


//
//
bedrockProject *bedrockPlugin::addProject ( const char *path, const char *root )
{
    return [owner addProject :path :root];
}
//
