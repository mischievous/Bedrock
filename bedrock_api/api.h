//
//  plugin.h
//  bedrock
//
//  Created by Alexandra Beebe on 7/30/22.
//

//
#include <functional>
#include <vector>
#include <string>

//
#include "bedrock.h"


#ifndef BEDROCK_API
#define BEDROCK_API

//
typedef std::function <void ( void )> callback_plugin;

//
class bedrockPlugin
{
    //
    private:
        NSBundle *bundle;
        bedrock  *owner;

    //
    protected:

        // windows/views
                id   addView   ( const char *nibName );

        // menus
                void addMenu   ( callback_plugin *callback, const std::vector <std::string> path );

        // projects
//                void addTarget ( 

    //
    public:
        //
                               bedrockPlugin ( NSBundle *bundle, bedrock *owner );
        virtual               ~bedrockPlugin ( void );
};


//
extern "C"
{
    typedef bedrockPlugin *( *pluginFactory_t ) ( NSBundle *bundle, bedrock *owner );
}


#endif /* plugin_h */
