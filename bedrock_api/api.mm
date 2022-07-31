//
//  plugin.m
//  plugin
//
//  Created by Alexandra Beebe on 7/30/22.
//

#include "api.h"


//
//
bedrockPlugin:: bedrockPlugin ( NSBundle *bundle, bedrock *owner ) : bundle(bundle), owner ( owner )
{
}
//


//
//
bedrockPlugin::~bedrockPlugin ( void )
{
}
//


//
//
id   bedrockPlugin::addView ( const char *nibName )
{
    return [owner addView:[[NSNib alloc] initWithNibNamed:[NSString stringWithUTF8String:nibName] bundle:bundle]];
}

//


//
//
void bedrockPlugin::addMenu ( callback_plugin *callback, const std::vector <std::string> path )
{
    //
    NSMenu *menu = [owner accessMenu];

    for (uint64_t idx = 0; idx < path.size(); idx += 1)
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

            if ( (idx + 1) < path.size() )
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
