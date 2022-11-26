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

//
#include "utilities.h"

//
#define SEP "/"


//
//
uint32_t bedrock_exists    ( const char *ipath )
{
    //
    struct stat buf;
    if (stat (ipath, &buf) != 0)
        return 0; // project does not exists.

    return 1;
}
//


//
//
uint32_t bedrock_directory ( const char *ipath )
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
void     bedrock_remove    ( const char *ipath )
{
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

                bedrock_remove ( path );
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
void     bedrock_copy      ( const char *s    , const char *d, uint64_t replace )
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
uint64_t bedrock_makedir   ( const char *ipath, mode_t mode )
{
    if (bedrock_directory ( ipath ))
        return 1; // Already exists..


    char path[1024];
    snprintf (path, sizeof (path), "%s", ipath);

    //
    for (char *object = strtok (path, SEP); object; object = strtok (NULL, SEP))
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
