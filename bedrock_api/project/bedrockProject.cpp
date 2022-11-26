//
//  bedrockProject.cpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

//
//#include <string.h>
#include <sys/param.h> // MIN/MAX
#include <fstream>
#include <strings.h>

//
#include "with.h"
#include "utilities.h"

//
#include "bedrockProject.h"


//
//
               bedrockProject:: bedrockProject ( const char *path, const char *root ) : workerQueue ("projectQueue")
{
    //
    memset (_name_, 0x0, sizeof (_name_));
    memset (_path_, 0x0, sizeof (_path_));
    memset (_proj_, 0x0, sizeof (_proj_));

    //
    snprintf (_path_, sizeof (_path_), "%s", path);
    bedrock_makedir( _path_ );

    //
    tokenizePath (path, &_abs_, "/");
    snprintf (_name_, sizeof (_name_), "%s", _abs_.back().c_str());

    //
    snprintf (_proj_, sizeof (_proj_), "%s/%s.bedrock", _path_, _name_);

    //
    if (root)
    {
        char path[1024];
        snprintf (path, sizeof (path), "%s/Library/Application Support/bedrock/platforms/%s.bedrock", getenv ("HOME"), root);
printf ("%s\n", path);

        bedrock_copy ( path, _proj_ );
    }
}
//


//
//
               bedrockProject::~bedrockProject ( void )
{

}
//


//
//
void           bedrockProject::remove          ( const char *workingPath )
{
    bedrock_remove ( workingPath );
}
//


//
//
void           bedrockProject::importPath      ( const char *key, nlohmann::json *object, v_args *working, const char *importPath )
{
    if (!(object->count (key)))
        return;

    v_args paths = (*object)[key];

    for (uint32_t idx = 0; idx < paths.size(); idx += 1)
    {
        //
        if (paths[idx][0] != '/')
            continue;

        char path[1024];
        snprintf (path, sizeof (path), "%s", paths[idx].c_str());

        relPath (path, working);
        if ( (!strncmp (path, "./", 2)) || (!strcmp (path, ".")) )
            (*object)[key].at (idx) = std::string (path);

        else if (!strncmp (path, "../", 3))
        {
            char tempPath[1024];
            snprintf (tempPath, sizeof (tempPath), "%s/%s", importPath, path);

            char *realPath;
            if ((realPath = realpath (tempPath, NULL)))
            {
                (*object)[key].at (idx) = std::string (realPath);
                free (realPath);
            }
        }

        else
            (*object)[key].at (idx) = std::string ("${PLATFORM}") + std::string (path);


//        paths[idx] = std::string (path);
    }

    //
//    (*object)[key] = paths;
}
//

//
//
void           bedrockProject::tokenizePath    ( const char *path, v_args *v, const char *sep )
{
    char worker[1024] = {0};
    snprintf (worker, sizeof (worker), "%s", path);

    for (char *object = strtok (worker, sep); object; object = strtok (NULL, sep))
        v->push_back(object);
}
//


//
//
void           bedrockProject::relPath         ( char *path, v_args *src )
{
    //
    const char *sep = "/";

    //
    v_args rel;
    tokenizePath (path, &rel, sep);


    //
    src = (src) ? src:&_abs_;

    // Get the shortest of the two paths
    size_t len = MIN(src->size(), rel.size());

    //
    uint64_t idx;
    for (idx = 0; idx < len; idx += 1)
    {
        const char *l = src->at(idx).c_str();
        const char *r = rel[idx].c_str();

        if (strcasecmp (l, r) == 0)
            continue;

        //
        if (idx == 0) // No common prex...
            return;

        break;
    }

    //
    memset (path, 0x0, 1024);

    //
    for (uint64_t jdx = idx; jdx < src->size(); jdx += 1)
    {
        strncat (path, "../", 1024);
    }

    //
    if (!strlen (path))
        strncat (path, ".", 1024);
    else
        path[strlen(path) - 1] = '\0';

    //
    for (uint64_t jdx = idx; jdx < rel.size(); jdx += 1)
    {
        strncat (path,              "/", 1024);
        strncat (path, rel[jdx].c_str(), 1024);
    }
}
//


//
//
void           bedrockProject::build_command   ( v_args *args, const char *opt, const char *key, nlohmann::json *object )
{
    //
    std::string owner = (*object)["owner"];
    if ( owner.length() != 0 )
        build_command ( args, opt, key, &_root_ [ owner ] );

printf ("bedrockProject::build_command : %s\n", ((*object)["target"].is_null() ? "ROOT":((std::string) (*object)["target"]).c_str() ));

    //
    for ( std::string path : (*object)[key] )
    {
        char value[1024];
        snprintf (value, sizeof (value), "%s", path.c_str());

        if ( (!strncmp (value, "./", 2)) || (!strcmp (value, ".")) )
        {
            snprintf (value, sizeof (value), "%s/%s", _path_, path.c_str());

            //
            char *realPath;
            if ((realPath = realpath (value, NULL)))
            {
                snprintf (value, sizeof (value), "%s", realPath);
                free (realPath);
            }
        }


        //
        char actual[1024];
        snprintf (actual, sizeof (actual), "%s%s", opt, value);

        //
        char *env;
        if ((env = strstr (actual, "${PLATFORM}")))
        {
            *env = 0x0; env += 11;
            char temp[1024];
            snprintf (temp, sizeof (temp), "%s%s%s", actual, "/Volumes/Promise/projects/bedrock/reference/platforms/centos.ppc", env);

            memset (actual, 0x0, sizeof (actual));
            memcpy (actual, temp, strlen (temp));
        }

        //
        args->push_back ( actual );
    }
}
//


//
//
void           bedrockProject::import          ( const char *workingPath )
{
    //
    if (!(bedrock_exists ( _proj_ )))
        return;

    //
    v_args working;
    tokenizePath (workingPath, &working, "/");

    //
    std::ifstream f(_proj_);
    _root_ = nlohmann::json::parse (f);


    // TODO
    workerQueue.addResource ( "localhost",  2 /* 20 */, "127.0.0.1"); // devel04
    workerQueue.addResource ( "localhost",  2 /* 20 */, "127.0.0.2"); // devel05
    workerQueue.addResource ( "localhost",  4 /* 32 */, "127.0.0.3"); // devel06
    workerQueue.addResource ( "localhost",  3 /* 26 */, "127.0.0.4"); // devel07
    workerQueue.addResource ( "localhost",  3 /* 26 */, "127.0.0.5"); // devel08
    workerQueue.addResource ( "localhost",  3 /* 26 */, "127.0.0.6"); // devel09

    //
    workerQueue.build();

workerQueue.pause();
for (uint32_t idx = 0; idx < 150; idx += 1)
    workerQueue.enqueue ([] ( resource_job * ) -> void { sleep (1); });
workerQueue.start(); 

    for (std::string key : _root_["objects"])
    {
        __uuid__ uuid;
        if ((6 != sscanf (key.c_str(), "%8x-%4hx-%4hx-%4hx-%8x%4hx", &uuid.i.u0, &uuid.i.u1, &uuid.i.u2, &uuid.i.u3, &uuid.i.u4, &uuid.i.u5)))
            continue;

        //
        nlohmann::json object = _root_[key];
        if (object["target"].is_null())
            continue; // ROOT??

        //
        if (object["actual"].is_null())
            continue;

//printf ("%s : %s\n", key.c_str(), ((std::string) object["target"]).c_str());


        // Correct the actual path to be "relative"
        std::string owner = object["owner" ];
        std::string temp  = object["actual"];

        char path[1024];
        snprintf (path, sizeof (path), "%s", temp.c_str());

        // clean target types of 5/7
        switch ( (uint64_t) object["targetType"] )
        {
            case PO_TARGET :
                // remove the actual file
                remove ( path );

                // no owner ... this is a root object
                if (owner.length() == 0)
                    _objects_[uuid.actual] = &object;
                break;

            case PO_SOURCE :
                // remove the actual file
                remove ( path );

                // no owner ... this is a "import warning"
                if (owner.length() == 0)
                {
                    printf ("WARNING : bedrockProject::import : %s\n", key.c_str());
                    continue;
                }

                //
                break;

            default        : break;
        }

        //
        object.erase("actual");

        //
        importPath ( "includePath" , &object, &_abs_, workingPath );
        importPath ( "libraryPath" , &object, &_abs_, workingPath );
//        importPath ( "dependancies", &object, &working );

//for (std::string path : object["includePath"] )
//    printf ("\t%s\n", path.c_str());
//printf ("\n");


        //
        // dependancies -- any dependancy starting with "/" is a import error
        for ( std::string dependancy : object["dependancies"] )
        {
            //
            if (dependancy[0] != '/')
                continue;

            //       
//            printf ("ERROR   : bedrockProject::import : %s : %s\n", key.c_str(), dependancy.c_str());
        }

        //
        _root_[key] = object;
    }


    //
    for (std::string key : _root_["objects"])
    {
        nlohmann::json object = _root_[key];

        //
        if ((object["targetType"] != PO_SOURCE))
            continue;

        //
        std::string owner = object["owner" ];
        if (owner.length() == 0)
        {
            printf ("WARNING : bedrockProject::import : %s\n", key.c_str());
            continue;
        }

        printf ("%s : %s\n", key.c_str(), ((std::string) object["target"]).c_str());
        
        //
//        _clang_.include_files ( &object );
        // TODO: Syntax check the object and record any errors found.
        //
    }


workerQueue.wait(); 

    //
    std::ofstream o(_proj_);
    o << std::setw(1) << _root_ << std::endl;
}
//


//
//
void           bedrockProject::clean           ( void )
{
    //
//    m_objects::iterator it;
//    for ( it = _objects_.begin(); it != _objects_.end(); it ++ )
//    {
//        //
//        uint64_t targetType = (*(*it).second)["targetType"];
//
//        switch (targetType)
//        {
//            case PO_TARGET : printf ("clean target {*.la} \n"); break;
//            case PO_SOURCE : printf ("clean target {*.lo} \n"); break;
//            default        : break;
//        }
//
//    }
}
//


//
//
void           bedrockProject::includePath    ( v_args *args, nlohmann::json *object )
{
    build_command   ( args, "-I", "includePath", object );
}
//


//
//
void           bedrockProject::defines        ( v_args *args, nlohmann::json *object )
{
    build_command   ( args, "-D", "defines"    , object );
}
//


//
//
void           bedrockProject::options        ( v_args *args, nlohmann::json *object )
{
    build_command   ( args, ""  , "options"    , object );
}
//


//
//
const char    *bedrockProject::target         ( nlohmann::json *object )
{
    //
    return NULL; // ((std::string) (*object)["path"]).c_str();
}
//

// FIXME
#if 0

//
//
projectObject *bedrockProject:: locObject      ( uint64_t type, const char *name, const char *path )
{
    //
    if ((name == NULL) && (path == NULL))
        return NULL;

    //
    m_objects::iterator it;

    for (it = _master_.begin(); it != _master_.end(); it++)
    {
        projectObject *object;
        if (!(object = (*it).second))
            continue;

        //
        if (object->type() != type)
            continue;

        //
        if ( (name) && (strcmp (object->title(), name)) )
            continue;

        if ( (path) && (strcmp (object->path() , path)) )
            continue;

        return object;
    }

    return NULL;
}
//
#endif


// FIXME
#if 0
//
//
projectObject *bedrockProject:: object         ( uint64_t type, const char *name, const char *path )
{
    return locObject (type, name, path);
}
//
#endif


// FIXME
#if 0
//
//
projectObject *bedrockProject:: addObject      ( uint64_t type, const char *path, const char *name, objects_t *dependancies, uint128_t uuid )
{
    //
    char workingPath [1024] = {0};
    snprintf (workingPath, sizeof (workingPath), "%s", path);

    // convert path to a relativePath for this fild...
    if (workingPath[0] == '/')
        relPath ( workingPath );

    //
    name = (name) ? name:strrchr (path, '/');

    //
    projectObject *child;
    if (!(child = locObject ( type, ACTUAL_NAME(name), workingPath )))
    {

        if (!(child = new projectObject ( type, workingPath, ACTUAL_NAME(name), this )))
            return NULL;
    }

    //
    if (uuid == 0x0)
        uuid = child->uuid().uuid;

    //
    if (_master_.find (uuid) == _master_.end())
        _master_[uuid] = child;

    //
    if (_root_)
    {
        switch (type)
        {
            case PO_TARGET : _root_->child ( child ); break;
        }
    }


    //
    if (dependancies)
    {
        for ( uint128_t dependancy : *dependancies )
            child->dependancy ( object (dependancy) );
    }


    //
    return child;
}
//
#endif

// FIXME
#if 0

//
//
void      bedrockProject::dependancies ( void )
{
    printf ("bedrockProject::dependancies\n");

    //
    m_objects::iterator it;

    for (it = _master_.begin(); it != _master_.end(); it++)
    {
        projectObject *object;
        if (!(object = (*it).second))
            continue;

        //
        printf ("\t%s [ %-64s ]\n", object->title(), object->path());
        _clang_.include_files (object->path());
    }
}
//
#endif
