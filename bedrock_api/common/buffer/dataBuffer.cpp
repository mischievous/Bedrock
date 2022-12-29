//
//  dataBuffer.cpp
//  spartacus
//
//  Created by Alexandra Beebe on 2/4/22.
//

//
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>


//
#include "dataBuffer.h"


//
//
          dataBuffer:: dataBuffer ( uint64_t size, uint64_t debug ) : _size_(2), _lngth_(0), buffer (NULL)
{
    reserve(size);
}
//

//
//
          dataBuffer::~dataBuffer ( void )
{
    //
    if (buffer) { free (buffer); buffer = NULL; }
}
//

//
//
void      dataBuffer::reserve     ( uint64_t size )
{
    if (size < _size_)
        return;

    //
    while (_size_ < size)
        _size_ *= 2;

    //
    buffer = (uint08_t *) realloc( buffer, _size_ );
}
//


//
//
void      dataBuffer::clear       ( uint64_t clearMemory )
{
    if (clearMemory)
        memset (buffer, 0x0, _lngth_);

    _lngth_ = 0;
}
//


//
//
char     *dataBuffer::operator+=  ( char *append )
{
    //
    uint64_t actual = (uint64_t) strlen (append);

    //
    grow (actual + 1);

    //
    memcpy (&buffer[_lngth_], append, actual);
    _lngth_ += actual;
    buffer[_lngth_] = 0x0;

    return (char *) buffer;
}
//


//
//
void      dataBuffer::append      ( const char *format, ... )
{
    va_list  args;

    //
    va_start (args, format);

    //
    uint64_t avail  = _size_ - _lngth_;
    uint64_t actual = vsnprintf ((char *) &buffer[_lngth_], avail, format, args);

    //
    va_end (args);


    if ( actual >= avail )
    {
        //
        grow (actual);

        //
        va_start (args, format);

        //
        avail  = _size_ - _lngth_;
        actual = vsnprintf ((char *) &buffer[_lngth_], avail, format, args);

        //
        va_end (args);
    }


    _lngth_ += actual;


}
//


//
//
void      dataBuffer::secure      ( const char *path )
{
    //
    FILE *file;
    if ((file = fopen (path, "wb")))
    {
        fwrite ((void *) &buffer[0], 1, _lngth_, file);
        fclose (file);
    }
}
//


//
//
void      dataBuffer::access      ( const char *path )
{
    struct stat buf;
    if (stat (path, &buf) != 0)
        return; // project does not exists.

    //
    if (!S_ISREG (buf.st_mode))
        return;

    //
    FILE *file;
    if ((file = fopen (path, "rb")))
    {
        //
        grow (buf.st_size + 1);

        //
        _lngth_  += fread ((void *) &buffer[_lngth_], 1, buf.st_size, file);
        fclose (file);
    }

    buffer[_lngth_] = 0x0;
}
//
