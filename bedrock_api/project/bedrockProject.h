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
        char           _name_ [  256 ];

        //
        projectObject *_root_;

        //
        objects_t      _master_;

    protected :


    public    :
                                bedrockProject ( const char *path );
        virtual                ~bedrockProject ( void );

                 //
                 const char    *name           ( void ) { return _name_; }

                 //
                 void          *root           ( void ) { return (void *) _root_; }
                 projectObject *addObject      ( uint64_t type, const char *name, const char *path = NULL, objects_t *dependancies = NULL );

        //
//                 uint64_t       count ( void         ) { return targets.size(); }
//                 void          *child ( uint64_t idx ) { if (idx >= targets.size()) return NULL; return (void *) targets[idx]; }

};

#endif /* bedrockProject_hpp */
