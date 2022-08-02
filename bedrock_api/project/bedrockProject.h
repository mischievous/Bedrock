//
//  bedrockProject.hpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

#ifndef BEDROCK_RPOJECT
#define BEDROCK_RPOJECT

//
#include "projectObject.h"

//
class bedrockProject
{
    private   :
        char name [ 1024 ];
        char path [ 1024 ];

        //
        projectObject *_root_;
//        objects_t      master;
//        objects_t      targets; // Linking
//        objects_t      source;  // Source files.

    protected :


    public    :
                                bedrockProject ( const char *path );
        virtual                ~bedrockProject ( void );

                 //
                 void          *root           ( void ) { return (void *) _root_; }
                 projectObject *addObject      ( uint64_t type, const char *name, objects_t *dependancies = NULL );

        //
//                 uint64_t       count ( void         ) { return targets.size(); }
//                 void          *child ( uint64_t idx ) { if (idx >= targets.size()) return NULL; return (void *) targets[idx]; }

};

#endif /* bedrockProject_hpp */
