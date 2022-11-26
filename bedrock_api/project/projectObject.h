//
//  projectObject.hpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

#ifndef PROJECTOBJECT
#define PROJECTOBJECT

//
#include <vector>
#include <string>

#include "uuid.h"

//
class projectObject;
typedef std::vector <uint128_t  > objects_t;
typedef std::vector <std::string> options_t;

typedef enum
{
    PO_ROOT     = 0,

    //
    PO_MAKEFILE    ,
    PO_XCODE       ,
    PO_PROJECT     ,

    PO_FOLDER      ,

    PO_TARGET      , // Application, Libraries, Console, Bundles, Archive

    PO_HEADER      , // *.h, *.hpp, ...
    PO_SOURCE      , // *.c, *.cpp, *.m, *.mm
    PO_ACTION      ,
    PO_IGNORE      , 
    PO_LAST

} PO_TYPE;

typedef struct
{
    __uuid__ uuid;

    uint64_t type;
    char     name[ 256];
    char     path[1024];

} objectData;

// forward declararion
class bedrockProject;

//
class projectObject
{
    private   :
        objectData         object;

        //
        bedrockProject    *owner;

//        char     *image;

        //
        projectObject     *parent;

        //
        options_t          options;

        //
        objects_t          children;
        objects_t          dependancies;

        //
                 bool      compare ( projectObject *lhs, projectObject *rhs );
        
    protected :


    public    :
                       projectObject ( uint64_t type, const char *path, const char *name, bedrockProject *owner = NULL );
        virtual       ~projectObject ( void );

                 //
                 uint64_t       count      ( void                  ) { return children.size(); }

                 void           dependancy ( projectObject *object );
                 void           child      ( projectObject *object );
                 projectObject *child      ( uint64_t idx          );



                 //
                 void           sort       ( void                  );

                 //
                 __uuid__       uuid       ( void                  ) { return object.uuid; }
                 uint64_t       type       ( void                  ) { return object.type; }
                 char          *path       ( void                  );
                 char          *title      ( void                  ) { return object.name; }
};

#endif /* projectObject_hpp */
