//
//  bedrockTask.cpp
//  plugin
//
//  Created by Alexandra Beebe on 8/3/22.
//

//
// https://www.demo2s.com/c/c-execlp-ls-ls-l-null-java.html

//
#include <unistd.h>
#include <sys/ioctl.h> // ioctl
#include <sys/filio.h> // FIONREAD

//
#include "bedrockTask.h"


#define PIPE_R (0)
#define PIPE_W (1)

//
//
bedrockTask:: bedrockTask              ( const char *cmd, const args_t argv, const args_t vars )
{
    //
    pipe (fdo);
    pipe (fde);

    pthread_mutex_init (&running, NULL);
    pthread_mutex_init (&console, NULL);

    //
    threads[0] = threads[1] = threads[2] = NULL;

    //
    task_args.push_back(cmd);

    //
    for (std::string arg : argv)
        task_args.push_back(arg);

    //
    for (std::string var : vars)
        task_vars.push_back(var);

    //
    for (uint32_t idx = 0; idx < 2; idx += 1)
    {
        output  [idx]  = 1;
        callback[idx]  = NULL;
        args[idx].type = idx; // stdout/stderr
        args[idx].task = this;
    }
}
//


//
//
bedrockTask::~bedrockTask              ( void )
{
    close (fdo[PIPE_R]);
    close (fde[PIPE_R]);

    pthread_mutex_destroy (&console);
    pthread_mutex_destroy (&running);

}
//


//
//
void        *bedrockTask::thread_wait   ( bedrockTask *task )
{
    //
    pthread_mutex_lock   (&task->running);
    pthread_yield_np();


//    printf ("+bedrockTask::worker_wait : %d\n", task->child );
    uint32_t status;
    pid_t    pid = 0;
    if ( (pid = waitpid (task->child, (int *) &status, 0)) == -1)
        perror ("bedrockTask::worker_wait : THREAD [ CHILD ] ");

    //
    pthread_mutex_unlock (&task->running);

    //
    if (pthread_join( task->threads[0], NULL )) perror ("bedrockTask::worker_wait : THREAD [ STDOUT ] ");
    if (pthread_join( task->threads[1], NULL )) perror ("bedrockTask::worker_wait : THREAD [ STDERR ] ");

    //    
//    printf ("-bedrockTask::worker_wait\n");

    //
    return (void *) NULL;
}
//


//
//
void        *bedrockTask::thread_worker ( thread_args *args )
{
    bedrockTask *task = (bedrockTask *) args->task;

    //
    char     omsg[8];
    snprintf (omsg, sizeof (omsg), "%c -->", args->type == 0 ? 'o':'e');

    //
    int      pipe = (args->type == 0) ? task->fdo[PIPE_R]:task->fde[PIPE_R];

    //
    uint64_t idx = 0;
    char     buffer[4096 * 2];

    //
    fd_set rfds;
    uint32_t maxSocket = pipe + 1;

    //
    do
    {
        //
        FD_ZERO (&rfds);
        FD_SET  (pipe, &rfds);

        //
        uint32_t ready;
        if ((ready = select (maxSocket, &rfds, NULL, NULL, NULL) == -1))
        {
            perror (__PRETTY_FUNCTION__);
            continue;
        }

        //
        if (FD_ISSET(pipe, &rfds))
            idx = task->read  ( pipe, idx, sizeof(buffer), buffer, omsg, args->type );

    } while (pthread_mutex_trylock(&task->running) != 0);

    //
    idx = task->read  ( pipe, idx, sizeof(buffer), buffer, omsg, args->type );

    //
    char *output = buffer;
    while ( (*output == ' ') || (*output == '\t') )
        output += 1;

    if (strlen(output))
    {
        //
        pthread_mutex_lock   (&task->console);

        //
        if (task->output[args->type])
        printf ("%s %s <--\n", omsg, output);

        //
        task->std_buffer[args->type].push_back(output);

        //
        pthread_mutex_unlock (&task->console);
    }

    //
    pthread_mutex_unlock(&task->running);

    return NULL;
}
//


//
//
uint32_t bedrockTask::avail  ( uint32_t pipe )
{
    uint32_t result = 0;

    if (ioctl (pipe, FIONREAD, &result) != 0)
        result = 0;

    //
    return result;
}
//


//
//
uint64_t bedrockTask::read   ( uint32_t pipe, uint64_t idx, uint64_t count, char *buffer, char *omsg, uint32_t type )
{
    while ( avail ( pipe ))
    {
        //
        char byte;
        if (::read (pipe, &byte, 1) == 0)
            continue;
        //
        buffer[idx++] = byte;
        buffer[idx  ] = '\0';

        if (idx >= (count - 1))
        {
            printf ("bedrockTask::read : overflow\n");
            idx = (count - 1);
        }

        //
        if (byte == '\n')
        {
            buffer[idx - 1] = '\0';

            char *output = buffer;
            while ( (*output == ' ') || (*output == '\t') )
                output += 1;

            pthread_mutex_lock   (&console);

            if (strlen(output))
            {
                //
                if (this->output[type])
                    printf ("%s %s <-- [ %4zu ]\n", omsg, output, strlen(output));

                if (callback[type])
                    (*callback[type]) (type, output);

                //
                std_buffer[type].push_back(output);
            }
            else
            {
                if (this->output[type])
                    printf ("\n");
            }

            //
            pthread_mutex_unlock (&console);
            
            idx  = 0;
            buffer[0] = '\0';
        }
    }

    //
    return idx;
}
//

//
//
void         bedrockTask::forked       ( void )
{
    //
//    printf ("bedrockTask::factory : forked %d [ %d ]\n", child, getpid());

    //
    pthread_create (&threads[2], NULL, (start) thread_wait  , (void *) this);

    // Wait until the thread_wait is setup
    while (pthread_mutex_trylock(&running) == 0)
        pthread_mutex_unlock(&running);

    close (fdo[PIPE_W]);
    close (fde[PIPE_W]);

    //
    args[0].type  = 0; // stdout
    args[0].task = this;
    pthread_create (&threads[0], NULL, (start) thread_worker, &args[0]);

    //
    args[1].type  = 1; // stderr
    args[1].task = this;
    pthread_create (&threads[1], NULL, (start) thread_worker, &args[1]);
}
//


//
//
uint64_t     bedrockTask::builder      ( char **argv, const args_t args )
{
    uint64_t idx = 0;

    //
    for ( std::string arg : args )
    {
        uint64_t length = arg.length() + 4;

        argv[ idx ] = (char *) malloc (length);
        snprintf (argv[idx++], length, "%s", arg.c_str());
    }

    argv [idx] = NULL;

    return idx;
}
//


//
//
void         bedrockTask::launch       ( const args_t task_args, const args_t task_vars )
{
    //
    if ((child = fork()) < 0)
    {
        perror ("bedrockTask::factory : FORK FAILED\n");

        delete this;
        return;
    }

    //
    if ( child )
    {
        forked();
        return;
    }

    close (fdo [PIPE_R]);
    close (fde [PIPE_R]);

    //
    dup2(fdo[PIPE_W], 1); // duplicate a file descriptor 1 => stdout
    dup2(fde[PIPE_W], 2); // duplicate a file descriptor 1 => stdout

    //
    if (task_wd.length())
        chdir (task_wd.c_str());

    //
    char     *argv [task_args.size() + 1];
    uint64_t  argc = builder (argv, task_args); argc += 0;


    char     *vars [task_vars.size() + 1];
    uint64_t  varc = builder (vars, task_vars); varc += 0; 


#if 0
char cwd[1024];
getcwd(cwd, sizeof (cwd));

write(fde[PIPE_W], "( ", 2);
write(fde[PIPE_W],  cwd, strlen(cwd));
write(fde[PIPE_W], " )\n", 3);

for (uint64_t i = 0; i < argc; i+= 1)
{
    if (i) write(fde[PIPE_W], " ", 1);
    write(fde[PIPE_W], argv[i], strlen(argv[i]));
}

write(fde[PIPE_W], "\n", 1);
for (uint64_t i = 0; i < varc; i+= 1)
{
    write(fde[PIPE_W], "[ ", 2);
    write(fde[PIPE_W], vars[i], strlen(vars[i]));
    write(fde[PIPE_W], " ]\n", 3);
}

write(fde[PIPE_W], "\n", 1);
write(fde[PIPE_W], "\n", 1);
#endif

    //
    if (execve (argv[0], argv, vars) == -1)
        perror ("bedrockTask::launch : ");
//        write(fde[PIPE_W], "bedrockTask::launch : execve\n", 29);

    //
    exit(0);
}
//


//
//
void         bedrockTask::env          ( const args_t vars )
{
    for (std::string var : vars)
        task_vars.push_back(var);
}
//


//
//
bedrockTask *bedrockTask::factory      ( const char *cmd, const args_t args, const args_t vars, uint32_t start ) // uint32_t eso, uint32_t ese, task_callback *so, task_callback *se )
{
    bedrockTask *task;
    if (!(task = new bedrockTask ( cmd, args, vars )))
        return NULL;

    //
    if (start)
        task->launch ( task->task_args, task->task_vars );

    //
    return task;
}
//

