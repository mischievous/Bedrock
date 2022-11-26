//
//  jobQueue.cpp
//  plugin
//
//  Created by Alexandra Beebe on 11/25/22.
//

//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//
#include "interface.h"

//
#include "jobQueue.h"
#include "with.h"

//
typedef guiQueue *( *guiFactory_t ) ( const char * );


//
//
void     *jobQueue::thread_worker ( resource_job *resource  )
{
    //
    jobQueue     *self     = resource->self;

    while (self->running)
    {
//        printf ("jobQueue::thread_worker :: waiting [ %-12s ]\n", resource->context);
        if (sem_wait ( self->semaphore ) != 0)
        {
            perror ("jobQueue::thread_worker : exiting : ");
            break;
        }

        //
        self->pause();

        t_job job;
//        printf ("jobQueue::thread_worker :: gather  [ %-12s ]\n", resource->context);
        job = self->dequeue();

        self->start();


//        printf ("jobQueue::thread_worker :: check   [ %-12s ]\n", resource->context);
        if (!self->running)
            break;

        //
//        printf ("jobQueue::thread_worker :: run     [ %-12s ]\n", resource->context);
        job (resource);
    }

    pthread_exit (NULL);
}
//


//
//
          jobQueue:: jobQueue ( const char *name )
{
    running   = 1;
    queueSize = 0x0;

    //
    pthread_mutex_init (&mutex_jobs , NULL);
    pthread_mutex_init (&mutex_state, NULL);

    //
    if (!(semaphore  = sem_open("jobQueue", O_CREAT|O_EXCL, 0700, 0x0)))
        perror ("jobQueue:: jobQueue : ");

    //
    sem_unlink("jobQueue");

    //
    guiFactory_t factory;
    if (!(factory = (guiFactory_t) plugin(PLUGIN_TYPE)))
        factory = guiQueue::factory;

    gui = factory (  name );
}
//


//
//
          jobQueue::~jobQueue ( void )
{
    // Signal to all running threads to shut the F*K down
    running = 0;

    // wait for all jobs to finish
    join ();

}
//


//
//
t_job     jobQueue::dequeue     ( void )
{
    with_mutex mtx (&mutex_jobs);

    t_job job = jobs.front(); jobs.erase( jobs.begin() );
printf ("jobQueue::dequeue : %lu\n", jobs.size());

    return job;
}
//

//
//
void      jobQueue::join        ( void )
{
}
//


//
//
void      jobQueue::wait        ( void )
{
    for (uint32_t count = (uint32_t) jobs.size(); count; count = (uint32_t) jobs.size())
    {
        while (count == jobs.size())
            sleep (1);

        printf ("jobQueue::wait : %lu [ %u ]\n", jobs.size(), count);
    }

}
//


//
//
void      jobQueue::addResource ( const char *name, uint32_t count, const char *context )
{
    //
    queueSize += count;

    //
    resources.push_back ( (resource_job *) malloc (sizeof (resource_job)));

    //
    resources.back()->count       = count;
    resources.back()->self        = this;
    snprintf (resources.back()->name   , sizeof (resources.back()->name   ), "%s", name   );
    snprintf (resources.back()->context, sizeof (resources.back()->context), "%s", context);

    //
    gui->addResource ( count, context);

}
//


//
//
void      jobQueue::build       ( void )
{
    uint32_t cols = (uint32_t) resources.size();

    //
    double rate       [cols];
    double accumulator[cols];

    //
    uint32_t minCount = 0;
    uint32_t count    = 0;
    for (uint32_t col = 0; col < cols; col += 1)
    {
        resource_job *resource  = resources[col];

        //
        accumulator[col]        = 0.0;
        rate       [col]        = resource->count;

        count                  += resource->count;
        minCount                = (minCount == 0) ? resource->count : ((minCount < resource->count) ? minCount:resource->count);
    }

    for (uint32_t col = 0; col < cols; col += 1)
        rate       [col] /= ((double) minCount);


    //
    uint32_t rows = count / cols;

    //
    for (uint32_t row = 0; (row < rows) && (count > cols); row += 1)
    {
        uint32_t total = 0;
        for (uint32_t col = 0; col < cols; col += 1)
        {
            accumulator[col] += rate[col];
            total             = 1;
        }

        while (total)
        {
            total = 0;
            for (uint32_t col = 0; col < cols; col += 1)
            {
                if (accumulator[col] < 1.0)
                    continue;

                //
                pthread_t thread;
                pthread_create (&thread, NULL, (void *(*)(void *)) jobQueue::thread_worker, resources[col] );
                threads.push_back ( thread );

                count -= 1;
                accumulator[col] -= 1.0;
                if (accumulator[col] >= 1.0)
                    total += 1;
            }
        }
    }
}
//


//
//
void      jobQueue::enqueue     ( t_job job )
{
    //
    if (!semaphore)
        return;

    //
    with_mutex mtx (&mutex_jobs);
    jobs.push_back ( job );
printf ("jobQueue::enqueue : %lu\n", jobs.size());

    //
    sem_post (semaphore);
}
//
