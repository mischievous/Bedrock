//
//  tss.h
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/14/22.
//

#ifndef THREAD_SPECIFIC_STORAGE
#define THREAD_SPECIFIC_STORAGE

//
#include <pthread.h>


template< typename T >
class thread_specific_ptr
{
    private:
        static pthread_key_t  key;
        static pthread_once_t key_once;


        static void bootstrap ( void )
        {
            pthread_key_create(&key, NULL);
        }

    public:
                 thread_specific_ptr ( T val ) { pthread_once(&key_once, bootstrap); pthread_setspecific( key, (void *) val ); }
        virtual ~thread_specific_ptr ( void  ) { pthread_setspecific( key, NULL ); }

        T get (void) {  return (T) pthread_getspecific ( key ); }

        //
        operator const T   ( void  ) { return (T) pthread_getspecific ( key ); }
        operator       T   ( void  ) { return (T) pthread_getspecific ( key ); }
        void     operator= ( const T v) { pthread_setspecific( key,  (void *) v ); }

        T operator-> (void)  { return (T) pthread_getspecific ( key ); }
};

template<class T>
pthread_key_t  thread_specific_ptr<T>::key      = 0;

template<class T>
pthread_once_t thread_specific_ptr<T>::key_once = PTHREAD_ONCE_INIT;


#endif /* tss_h */
