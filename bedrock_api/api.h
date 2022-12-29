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
#include "with.h"


//
#include "bedrock.h"

//
#include "bedrock_uuid.h"


#ifndef BEDROCK_API
#define BEDROCK_API

//
typedef std::function <void ( void )> callback_plugin;

//
class bedrockPlugin
{
    //
    private:

    //
    protected:
        NSBundle *bundle;
        bedrock  *owner;
        void     *library;

        // plugins
                void            plugin       ( bedrock_uuid uuid, const char *name );
                void           *plugin       ( bedrock_uuid uuid );


        // windows/views
                id              addView      ( const char *nibName );

        // menus
                void            addMenu      ( callback_plugin *callback, const std::vector <std::string> path );
                void            performMenu  ( const std::vector <std::string> path );

        // projects
//                bedrockProject *addProject   ( const char *path, const char *root = NULL );
//                void addTarget ( 

    //
    public:
        //
                               bedrockPlugin ( NSBundle *bundle, void *library );
        virtual               ~bedrockPlugin ( void );
};


//
extern "C"
{
    typedef bedrockPlugin *( *pluginFactory_t ) ( NSBundle *bundle, void *library );
}


#endif /* plugin_h */
