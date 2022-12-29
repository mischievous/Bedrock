//
//  with.h
//  bedrock
//
//  Created by Alexandra Beebe on 11/11/22.
//

#ifndef WITH_TEMPLATE
#define WITH_TEMPLATE

//
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


template<typename T>
class with_template
{
    private   :
        T key;

    protected :
        //
        T        get           ( void ) { return key; }
        void     set           ( T  v ) { key = v;    }

    public    :
                 with_template (T key ) : key(key) {;}
        virtual ~with_template ( void ) {;}

//        operator const bool ( void ) { return (key != NULL); }
        operator const T    ( void ) { return (T) key; }
};

//
class with_mutex : public with_template<pthread_mutex_t *>
{
    public     :
                 with_mutex ( pthread_mutex_t *mtx ) : with_template<pthread_mutex_t *>(mtx) { pthread_mutex_lock ( mtx ); }
        virtual ~with_mutex ( void )                                                         { pthread_mutex_unlock ( this->get() );  };
};

//
class with_semaphore : public with_template<sem_t *>
{
    public     :
                 with_semaphore ( sem_t *s ) : with_template<sem_t *>(s) { sem_wait ( s ); }
        virtual ~with_semaphore ( void     )                             { sem_post ( this->get() );  };
};

//
//
class with_file : public with_template<FILE *>
{
    public     :
        //
        typedef size_t (*fn_action) ( void *ptr, size_t size, size_t nitems, FILE *stream );


    private    :
        size_t rtn;

    public     :
        size_t   result ( void )  { return rtn; }

        //
        // operator const FILE   *() { return this->get(); }

        //
        virtual ~with_file ( void )
                 {
                    fclose ( this->get() );
                 };


                 with_file ( const char *filename, const char *mode, fn_action action = NULL, const char *buffer = NULL, size_t length = 0 ) : with_template<FILE *> ( fopen (filename, mode) )
                 {
                    FILE *stream = get();

                    //
                    rtn = -1;
                    if ((stream != NULL) && (action != NULL) && (buffer != NULL) && (length != 0))
                        rtn = action ((void *) buffer, 1, length, stream);
                 }
};

//
class with_open : public with_template<uint32_t>
{
    public     :
                 with_open ( const char *filename, int oflag ) : with_template<uint32_t> ( open (filename, oflag) ) { ; }
        virtual ~with_open ( void )                                                                           { close (  this->get() ); };

        operator uint32_t ()  { return this->get(); }

};


#endif /* WITH_TEMPLATE */
