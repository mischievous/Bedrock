//
//  utilities.c
//  plugin
//
//  Created by Alexandra Beebe on 11/22/22.
//

//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <memory.h>
#include <sys/param.h> // MIN/MAX

//
#include <string>
#include <vector>


// bedrock_executable
//#include <stdlib.h>
//#include <sys/syslimits.h>
//#include <mach-o/dyld.h>

//
#include "utilities.h"


//
typedef std::vector < std::string > v_tokenize;


//
#define SEP "/"


//
//
uint64_t    tokenizePath   ( const char *path, v_tokenize *v, const char *sep )
{
    char worker[1024] = {0};
    snprintf (worker, sizeof (worker), "%s", path);

    //
    sep = (sep) ? sep:SEP;

    //
    uint64_t count = 0;
    for (char *object = strtok (worker, sep); object; object = strtok (NULL, sep))
    {
        count += 1;

        //
        if (!strcmp (object, "."))
            continue;

        //
        if (!strcmp (object, ".."))
        {
            v->erase ( v->end() - 1);
            continue;
        }

        v->push_back(object);
    }

    return (v->size() == count);
}
//

//
//
uint64_t    bedrock_exists    ( const char *ipath, uint64_t length )
{
    //
    struct stat buf;
    if (stat (ipath, &buf) != 0)
        return 0; // project does not exists.

    uint64_t rtn = 1;
    if ((S_ISREG (buf.st_mode)) && (length))
        rtn = buf.st_size;


    return rtn;
}
//


//
//
uint64_t    bedrock_exists    ( const FILE *file, uint64_t length )
{
    //
    struct stat buf;
    if (fstat ( fileno( (FILE *) file), &buf) != 0)
        return 0; // project does not exists.

    uint64_t rtn = 1;
    if ((S_ISREG (buf.st_mode)) && (length))
        rtn = buf.st_size;


    return rtn;
}
//


//
//
uint32_t    bedrock_directory ( const char *ipath )
{
    //
    struct stat buf;
    if (stat (ipath, &buf) != 0)
        return 0; // project does not exists.

    return S_ISDIR (buf.st_mode);
}
//


//
//
void        bedrock_remove    ( const char *ipath, uint64_t directory )
{
    if (!ipath)
        return;

    //
    if ( directory == 0 )
    {
        if ( bedrock_directory (ipath) )
            return;
    }

    //
    if ( bedrock_directory (ipath) )
    {
        DIR *dir;
        if ((dir = opendir(ipath)))
        {
            //
            struct dirent *entry = NULL;
            while ( (entry = readdir(dir)) )
            {
                if ((!strcmp (entry->d_name, ".")) || (!strcmp (entry->d_name, "..")))
                    continue;

                char path[1024];
                snprintf (path, sizeof (path), "%s/%s", ipath, entry->d_name);

                bedrock_remove ( path, 1 );
            }

            closedir (dir);
        }
    }

    //
    ::remove(ipath);
}
//


//
//
void        bedrock_copy      ( const char *s    , const char *d, uint64_t replace )
{
    if ((!bedrock_exists (s)))
        return;

    if (( bedrock_exists (d) && (!replace) ))
        return;

    FILE *sf = (FILE *) fopen (s, "rb");
    FILE *df = (FILE *) fopen (d, "wb");

    if ((sf) && (df))
    {
        char buffer[4096];

        while (!feof (sf))
        {
            size_t count;
            if (!(count = fread (buffer, 1, sizeof (buffer), sf)))
                continue; // read error, or feof

            if (fwrite (buffer, 1, count, df) != count)
                break; // wrtie error
        }
    }

    //
    if (sf) fclose (sf);
    if (df) fclose (df);
}
//

//
// code to make sure the directory exits
uint64_t    bedrock_makedir   ( const char *ipath, mode_t mode, const char *sep )
{
    if (bedrock_directory ( ipath ))
        return 1; // Already exists..


    char path[1024];
    snprintf (path, sizeof (path), "%s", ipath);

    //
    sep = (sep) ? sep:SEP;

    //
    for (char *object = strtok (path, sep); object; object = strtok (NULL, sep))
    {
        if ( !bedrock_directory (path) )
        {
            if (::mkdir(path, mode) != 0)
                return 0; // Error occured....
        }

        object[strlen(object)] = SEP[0];
    }

    //
    return bedrock_directory ( ipath );
}
//


//
//
const char *bedrock_realpath  (       char *ipath, uint32_t count )
{
    char wpath[PATH_MAX];
    memcpy (wpath, ipath, sizeof (wpath));
    
    //
    v_tokenize path;
    if (tokenizePath   ( wpath, &path, 0x0 ) == 0)
    {
        //
        memset (ipath,  0x0, count);

        //
        for ( std::string object : path )
        {
            strncat (ipath,            "/", count);
            strncat (ipath, object.c_str(), count);
        }
    }

    //
    return ipath;
}
//
