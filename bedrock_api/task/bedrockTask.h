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
typedef std::function< void ( uint32_t, char * ) > task_callback;

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
        pid_t                      child;

        //
        int                        fdo        [2]; // stdout
        int                        fde        [2]; // stderr

        //
        thread_args                args       [2];

        //
        pthread_mutex_t            running;
        pthread_mutex_t            console;

        //
        pthread_t                  threads    [3];

        uint32_t                   output     [2];
        task_callback             *callback   [2];
        std::vector <std::string>  std_buffer [2];

        //
//        std::string                task_cmd;
        args_t                     task_args;
        args_t                     task_vars;

        //
        std::string                task_wd;


        //
                             bedrockTask   ( const char *cmd, const args_t args, const args_t vars = args_t() );

        //
        static   void       *thread_wait   ( bedrockTask *task );
        static   void       *thread_worker ( thread_args *args );

        //
                 uint32_t    avail         ( uint32_t pipe   );
                 uint64_t    read          ( uint32_t pipe , uint64_t idx, uint64_t count, char *buffer, char *omsg, uint32_t type );

                 void        forked        ( void );
                 void        launch        ( const args_t task_args, const args_t task_vars );

                 uint64_t    builder       ( char **argv, const args_t args ); 

    protected :


    public    :
        //
        static  bedrockTask *factory           ( const char *cmd, const args_t args, const args_t vars = args_t(), uint32_t start = 1 );

        //
        virtual             ~bedrockTask       ( void );

                //
                void         env               ( const args_t vars ); 


        //
                void         so                ( task_callback *action = NULL, uint32_t enableOutput = 0 ) { output[0] = enableOutput; callback[0] = action; }
                void         se                ( task_callback *action = NULL, uint32_t enableOutput = 0 ) { output[1] = enableOutput; callback[1] = action; }

                void         cwd               ( const char *wd ) { task_wd = wd; }
                void         wait              ( void ) { if (threads[2] == NULL) { launch ( task_args, task_vars ); } pthread_join (threads[2], NULL); }
};

#endif /* bedrockTask_hpp */
