//
//  jobQueue.hpp
//  plugin
//
//  Created by Alexandra Beebe on 11/25/22.
//

#ifndef JOBQUEUE
#define JOBQUEUE

//
#include <functional>
#include <semaphore.h>
#include <vector>

#include "quiQueue.h"

//
class jobQueue;

//
typedef struct
{
    uint32_t    count;

    //
    char        name   [128];
    char        context[128];

    //
    jobQueue     *self;

} resource_job;

//
typedef std::function < void ( resource_job * ) > t_job;
typedef std::vector   < t_job                   > v_job;
typedef std::vector   < resource_job *          > v_job_resource;
typedef std::vector   < pthread_t               > v_threads;

//
class jobQueue
{
    private   :
        uint64_t         running;

        //
        guiQueue        *gui;

        //
        pthread_mutex_t  mutex_state;
        pthread_mutex_t  mutex_jobs;
        v_threads        threads;

        //
        uint32_t         queueSize;
        sem_t           *semaphore;

        v_job_resource   resources;
        v_job            jobs;

    protected :

    public    :

    private   :
        //
        static void      *thread_worker ( resource_job *resource  );

        //
        t_job             dequeue       ( void );

    protected :

    public    :

        //
                          jobQueue      ( const char *name );
        virtual          ~jobQueue      ( void );

        //
                void      join          ( void );
                void      wait          ( void );

        //
                void      pause         ( void ) { pthread_mutex_lock   ( &mutex_state ); }
                void      start         ( void ) { pthread_mutex_unlock ( &mutex_state ); }

        //
                void      addResource   ( const char *name, uint32_t count, const char *context );

        //
                void      build         ( void );

        //
                void      enqueue       ( t_job job );
};


#endif /* jobQueue_hpp */
