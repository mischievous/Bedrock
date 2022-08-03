//
//  bedrockTask.hpp
//  plugin
//
//  Created by Alexandra Beebe on 8/3/22.
//

//
#include <vector>
#include <string>

//
#include <pthread.h>

#ifndef BEDROCKTASK
#define BEDROCKTASK


//
typedef void *(*start) (void *);

//
typedef std::vector <std::string> args_t;

//
typedef struct
{
    int   type;
    void *task;

} thread_args;


class bedrockTask
{
    private   :
        //
        pid_t           child;

        //
        int             fdo     [2]; // stdout
        int             fde     [2]; // stderr

        //
        thread_args     args    [2];

        //
        pthread_mutex_t running;
        pthread_mutex_t console;

        //
        pthread_t       threads [3];

        //
                             bedrockTask  ( const char *cmd, const args_t args );

        //
        static   void       *thread_wait   ( bedrockTask *task );
        static   void       *thread_worker ( thread_args *args );

        //
                 uint32_t    avail         ( uint32_t pipe   );
                 uint64_t    read          ( uint32_t pipe, uint64_t idx, char *buffer, char *omsg );

    protected :


    public    :
        static  bedrockTask *factory      ( const char *cmd, const args_t args );
        virtual             ~bedrockTask  ( void );

        //
                void         wait         ( void ) { pthread_join (threads[2], NULL); }
};

#endif /* bedrockTask_hpp */
