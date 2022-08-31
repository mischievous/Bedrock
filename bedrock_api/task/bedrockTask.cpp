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
bedrockTask:: bedrockTask              ( const char *cmd, const args_t args )
{
    pipe (fdo);
    pipe (fde);

    pthread_mutex_init (&running, NULL);
    pthread_mutex_init (&console, NULL);
}
//


//
//
bedrockTask::~bedrockTask              ( void )
{
}
//


//
//
void        *bedrockTask::thread_wait   ( bedrockTask *task )
{
    //
    pthread_mutex_lock   (&task->running);
    pthread_yield_np();


    printf ("+bedrockTask::worker_wait : %d\n", task->child );
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
    printf ("-bedrockTask::worker_wait\n");

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
    char     buffer[4048];

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
            idx = task->read  ( pipe, idx, buffer, omsg );

    } while (pthread_mutex_trylock(&task->running) != 0);

    //
    idx = task->read  ( pipe, idx, buffer, omsg );

    //
    if (strlen(buffer))
    {
        pthread_mutex_lock   (&task->console);
        printf ("%s %s <--\n", omsg, buffer);
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
uint64_t bedrockTask::read   ( uint32_t pipe, uint64_t idx, char *buffer, char *omsg )
{
    while (avail ( pipe ))
    {
        //
        char byte;
        if (::read (pipe, &byte, 1) == 0)
            continue;

        if (byte == '\n')
        {
            pthread_mutex_lock   (&console);
            
            if (strlen(buffer))
                printf ("%s %s <--\n", omsg, buffer);
            else
                printf ("\n");

            pthread_mutex_unlock (&console);
            idx  = 0;
            buffer[0] = '\0';
            continue;
        }

        //
        buffer[idx++] = byte;
        buffer[idx  ] = '\0';
    }

    //
    return idx;
}
//


//
//
bedrockTask *bedrockTask::factory      ( const char *cmd, const args_t args )
{
    bedrockTask *task;
    if (!(task = new bedrockTask ( cmd, args )))
        return NULL;

    //
    if ((task->child = fork()) < 0)
    {
        perror ("bedrockTask::factory : FORK FAILED\n");

        delete task;
        return NULL;
    }

    //
    if ( task->child )
    {
        printf ("bedrockTask::factory : forked %d [ %d ]\n", task->child, getpid());

        //
        pthread_create (&task->threads[2], NULL, (start) thread_wait  , (void *) task);

        // Wait until the thread_wait is setup
        while (pthread_mutex_trylock(&task->running) == 0)
            pthread_mutex_unlock(&task->running);

        close (task->fdo[PIPE_W]);
        close (task->fde[PIPE_W]);

        //
        task->args[0].type  = 0; // stdout
        task->args[0].task = task;
        pthread_create (&task->threads[0], NULL, (start) thread_worker, &task->args[0]);

        task->args[1].type  = 1; // stderr
        task->args[1].task = task;
        pthread_create (&task->threads[1], NULL, (start) thread_worker, &task->args[1]);

        return task;
    }

//
chdir ("/Volumes/Promise/projects/bedrock/reference/legion/vehGraph");

    close (task->fdo [PIPE_R]);
    close (task->fde [PIPE_R]);

    //
    dup2(task->fdo[PIPE_W], 1); // duplicate a file descriptor 1 => stdout
    dup2(task->fde[PIPE_W], 2); // duplicate a file descriptor 1 => stdout

    //
    uint64_t  argc = 0;
    char     *argv [args.size() + 2];

    //
    argv [argc] = (char *) malloc (strlen (cmd) + 4);
    snprintf (argv[argc++], strlen (cmd) + 4, "%s", cmd);
    //
    for (uint64_t idx = 0; idx < args.size(); idx += 1)
    {
        uint64_t length = args[idx].length() + 4;

        argv[argc] = (char *) malloc (length);
        snprintf (argv[argc++], length, "%s", args[idx].c_str());
    }

    //
    argv [argc] = NULL;

char cwd[1024];
getcwd(cwd, sizeof (cwd));

printf ("%s\n", cwd);
for (uint64_t i = 0; i < argc; i+= 1)
{
    if (i) printf (" ");

    printf ("%s", argv[i]);
}
printf ("\n");
printf ("\n");


    //
    if (execve (argv[0], argv, NULL) == -1)
        perror ("bedrockTask::factory : execve ");

    //
    exit(0);
}
//

