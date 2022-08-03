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


//
class projectObject;
typedef std::vector <projectObject *> objects_t;
typedef std::vector <std::string>     options_t;

typedef enum
{
    PO_ROOT     = 0,

    //
    PO_MAKEFILE    ,
    PO_XCODE       ,
    PO_PROJECT     ,

    PO_FOLDER      ,

    PO_TARGET      , // Application, Libraries, Console, Bundles

    PO_HEADER      , // *.h, *.hpp, ...
    PO_SOURCE      , // *.c, *.cpp, *.m, *.mm
    PO_LAST

} PO_TYPE;

//
class projectObject
{
    private   :
        uint64_t  type;
        char      name        [256];
        char      path        [256];
//        char     *image;
        void     *cell;

        //
        objects_t children;

        //
        objects_t dependancies;
        options_t options;

        //
        static   bool      compare ( projectObject *lhs, projectObject *rhs );

        
    protected :


    public    :
                       projectObject ( uint64_t type, const char *name, const char *path = NULL, objects_t *dependancies = NULL );
        virtual       ~projectObject ( void );

        //
                 uint64_t  count     ( void                  ) { return children.size(); }
                 void      child     ( projectObject *object ) { children.push_back(object); }
                 void     *child     ( uint64_t idx          ) { if (idx >= children.size()) return NULL; return (void *) children[idx]; }

        //
                 void      sort      ( void                  );

        //
                 char     *title     ( void                  ) { return name; }

        //
                 void     *view      ( void                  ) { return cell; }
                 void      view      ( void *view            ) { cell = view; }
};

#endif /* projectObject_hpp */
