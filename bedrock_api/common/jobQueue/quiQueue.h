//
//  jobQueueGui.h
//  bedrock
//
//  Created by Alexandra Beebe on 11/26/22.
//

#ifndef GUIQUEUE
#define GUIQUEUE

#include "bedrock_uuid.h"

//
#define PLUGIN_TYPE ( bedrock_uuid {.d064.u0 = 0xc34bcfcb4e7ad414, .d064.u1 = 0xdc86856164af2adf } )


//
class guiQueue
{
    private   :
    protected :
    public    :

    private   :
    protected :
                              guiQueue    ( void  ) { ; }

    public    :
        static  guiQueue     *factory     ( const char *name ) { return new guiQueue ( ); }
        virtual              ~guiQueue    ( void )             { ; }

        //
        virtual void          addResource ( uint32_t count, const char *context ) { ; }

};


#endif /* jobQueueGui_h */
