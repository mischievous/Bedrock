//
//  bedrockProject.hpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

//
#include <map>

//
#ifndef BEDROCK_RPOJECT
#define BEDROCK_RPOJECT

//
#include "json.hpp" // https://github.com/nlohmann/json


//
#include "bedrockTypes.h"
#include "jobQueue.h"
#include "projectObject.h" // PO_TYPE
//#include "clang.h"

#define ACTUAL_NAME(name) (name[0] != '/') ? &name[0]:&name[1]


//
typedef std::map    <uint128_t, nlohmann::json *> v_objects;
//typedef std::vector <           std::string     > v_path;
//typedef std::vector <           std::string     > v_args;


//
class bedrockProject
{
    private   :
        //
        jobQueue       workerQueue; 
//        bedrockClang   _clang_;

        nlohmann::json _project_;

        //
        v_args         _abs_;
        char           _name_ [  256 ];
        char           _path_ [ 1024 ];
        char           _proj_ [ 1024 ]; // full project name (path/name.bedrock)

        //             uuid of root objects
        v_objects      _objects_;

        //
        nlohmann::json _root_;

    protected :

    public    :


    private   :
                 void           importPath     ( const char *key, nlohmann::json *object, v_args *working, const char *importPath );

                 void           tokenizePath   ( const char *path, v_args *v, const char *sep );
                 void           relPath        ( char *path      , v_args *src = NULL );

                 //
                 void            build_command ( v_args *args, const char *opt, const char *key, nlohmann::json *object );

//                 projectObject *locObject      ( uint64_t type, const char *name, const char *path );

    protected :

    public    :
                                bedrockProject ( const char *path, const char *root );
        virtual                ~bedrockProject ( void );

        //
                void            remove         ( const char *workingPath );

        //
                void            load           ( void ); // Load the project file
                void            import         ( const char *workingPath ); 

        // actions
                void            clean          ( void );

                 //
                 const char    *name           ( void ) { return _name_; }
                 const char    *path           ( void ) { return _path_; }
                 const char    *project        ( void ) { return _proj_; }

                 //
                 void           dependancy     ( const char *dependancy, nlohmann::json *object ) { printf ("dependancy : %s\n", dependancy); }

                 //
                 void           includePath    ( v_args *args, nlohmann::json *object );
                 void           defines        ( v_args *args, nlohmann::json *object );
                 void           options        ( v_args *args, nlohmann::json *object );
                 const char    *target         ( nlohmann::json *object );


                 //
//                 projectObject *root           ( void ) { return _root_; }
//
//                 //
//                 projectObject *object         ( uint128_t key )
//                 {
//                     if (_master_.find (key) != _master_.end())
//                        return _master_[key];
//
//                     return NULL;
//                 }
//
//                 //
//                 projectObject *object         ( uint64_t type, const char *name, const char *path = NULL);
//                 projectObject *addObject      ( uint64_t type, const char *path, const char *name = NULL, objects_t *dependancies = NULL, uint128_t uuid = 0 );
//
//                 //
//                 void           dependancies   ( void );


};

#endif /* bedrockProject_hpp */
