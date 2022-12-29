//
//  logger.cpp
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/16/22.
//

//
// #include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include <vector>


//
#include <CoreFoundation/CoreFoundation.h>
//CFShow
//CFStringCreateWithBytesNoCopy
//CFRelease

//
#include "crc.h"


//
#include "logger.h"

//
#include "with.h"


typedef enum
{
    FT_C          = 0, // C function
    FT_CPP_STATIC    , // cpp  class function
    FT_CPP           , // cpp        functions
    FT_OBJC_CLASS    , // OBJC class function (aka +)
    FT_OBJC          , // OBJC       function (aka -)

} FUNTION_TYPES;


//
pthread_mutex_t mutex;
sem_t          *semaphore;

//
std::vector < logger_payload > payloads;


//
m_loggers       remoteLogging::loggers;
udp_address     remoteLogging::address = { .length = 0, .sckt = 0 };

//
uint32_t        remoteLogging::global_level   = LEVEL_ALL;
uint32_t        remoteLogging::global_options = 0x0;
uint32_t        remoteLogging::depth          = 0x0;


//
//
uint64_t hash        ( const char *value )
{
    uint64_t rtn = 0x0;

    if (value)
        rtn = F_CRC_CalculaCheckSum ((uint08_t *) value, strlen (value));

    return rtn;
}
//




//
//
__attribute__((constructor)) void           remoteLogging::bootstrap     ( void )
{
    pthread_t       worker;

    //
    pthread_mutex_init (&mutex, NULL);

    //
    if (!(semaphore  = sem_open("remoteLogging", O_CREAT|O_EXCL, 0700, 0x0)))
    {
        perror ("remoteLogging:: bootstrap : ");
        exit (1);
    }

    //
    sem_unlink("remoteLogging");

    //
    pthread_create(&worker, NULL, (void *(*) (void *)) remoteLogging::worker, NULL);
}
//


//
//
void          *remoteLogging::worker ( void )
{
    char threadName[256];
    snprintf (threadName, sizeof (threadName), "remoteLogging::worker");

    if (pthread_setname_np( threadName ))
        perror ("remoteLogging::worker - pthread_setname_np");

    //
    while (1)
    {
        //
        if (sem_wait ( semaphore ) != 0)
        {
            perror ("remoteLogging::worker : exiting : ");
            break;
        }

        pthread_mutex_lock   ( &mutex );
        logger_payload payload;
        memcpy (&payload, &payloads.front(), sizeof (logger_payload)); payloads.erase ( payloads.begin() );
        pthread_mutex_unlock ( &mutex );


//        if (!strlen (payload.message))
//            continue;


        //
        char message[sizeof (logger_payload::message)];
        size_t length = 0;

        //
        if (payload.depth > 1)
        {
            snprintf (&message[0], sizeof (message), "%-*s", payload.depth - 1, "");
            length = depth - 1;
        }

        //
        snprintf (&message[length], sizeof (logger_payload::message ) - length, "%c", payload.prologue);
        length += 1;

        //
        snprintf (&message[length], sizeof (logger_payload::message ) - length, "%*s::", (int) sizeof (logger_payload::classname ), strlen (payload.classname) ? payload.classname:"");
        length  = strlen (message);

        //
        snprintf (&message[length], sizeof (logger_payload::message ) - length, "%-*s", (int) sizeof (logger_payload::function ), strlen (payload.function) ? payload.function:"");
        length  = strlen (message);


        snprintf (&message[length], sizeof (logger_payload::message ) - length, " %s", strlen (payload.message) ? payload.message:"");
        length  = strlen (message);

        //
        snprintf (&message[length], sizeof (logger_payload::message ) - length, " %c", payload.epilogue);
        length += 1;

        //
        if (!length)
            continue;

printf ("%s\n", message);
    }

    pthread_exit(0);
}
//


//
//
void           remoteLogging::enqueue       ( logger_payload *payload )
{
    with_mutex mtx (&mutex);

    //
    payloads.push_back ( *payload );

    //
    sem_post (semaphore);
}
//


//
//
remoteLogging *remoteLogging::logger  ( const char *file )
{
    uint64_t owner = 0x0;

    if (file)
        owner = hash (file);

    if (loggers.find (owner) != loggers.end())
        return loggers[owner];

    //
    if (owner == 0x0)
        return NULL;

    //
    return logger (NULL);
}
//


//
//
remoteLogging *remoteLogging::factory ( const char *file, const char *category, uint32_t level, uint32_t options )
{
if (file == NULL)
    printf ("remoteLogging::factory : %08lx\n", sizeof (logger_payload));


    uint64_t owner = 0;

    if (file)
        owner = hash (file);


    // check for errors...
//    if ((owner == 0) && (level == LEVEL_GLOBAL))
//    {
//        exit (0); // fatal error...
//    }


    //
    if (loggers.find (owner) == loggers.end())
        loggers[owner] = new remoteLogging ( owner, category, level, options );

    //
    return loggers[owner];
}
//


//
//
void           remoteLogging::cpp      (logger_payload *payload, const char *function)
{
    char local[256];
    snprintf (local, sizeof (local), "%s", function);

    // c++ class function
    payload->functionType = FT_CPP_STATIC; //

    //
    char *worker;
    if (!(worker = strstr (local, "static")))
    {
        payload->functionType = FT_CPP;
        worker = local;
    }


    char *temp;
    if (!(temp = (char *) strchr (worker, '(')))
    {
        // fatal error... log it...
        return;
    }

    *temp = '\0';
    if ((temp = (char *) strchr (worker, ':')))
    {
        //
        while (*temp == ':')
            *temp++ = '\0';

        snprintf (payload->function , sizeof (logger_payload::function ), "%s", temp);

        //
        if (!(temp = strrchr  (worker, ' ')))
            temp = local;

        //
        while (*temp == ' ')
            *temp++ = '\0';


        //
        snprintf (payload->classname, sizeof (logger_payload::classname), "%s", temp);

        return;
    }

    // C Function
    payload->functionType = FT_C; //

    //
    if (!(temp = strrchr  (worker, ' ')))
        temp = local;

    //
    while (*temp == ' ')
        *temp++ = '\0';


    //
    snprintf (payload->function, sizeof (logger_payload::function), "%s", temp);


    //
//    printf ("  %s::%s\n", payload->classname, payload->function);
}
//


//
//
void           remoteLogging::objc     (logger_payload *payload, const char *function)
{
    char local[256];
    snprintf (local, sizeof (local), "%s", function);

    //
    payload->functionType = local[0] == '+' ? FT_OBJC_CLASS:FT_OBJC; //

    char *temp;
    if (!(temp = (char *) strchr (local, ':')))
    {
        if (!(temp = (char *) strchr (local, ']')))
        {
            if ((temp = (char *) strchr (local, '(')))
            {
                remoteLogging::cpp (payload, function);
                return;
            }

            exit(1);
            // fatal error... log it...
            return;
        }
    }

    *temp = '\0';
    if (!(temp = (char *) strchr (local, ' ')))
    {
        exit(1);

        // fatal error... log it...
        return;
    }

    *temp++ = '\0';

    //
    snprintf (payload->classname, sizeof (logger_payload::classname), "%s", &local[2]);
    snprintf (payload->function , sizeof (logger_payload::function ), "%s", temp);

    //
//    printf ("%c %s::%s\n", (payload->functionType == FT_OBJC) ? '-':'+', payload->classname, payload->function);
}
//


//
//
uint64_t       remoteLogging::getLevel ( uint32_t test, remoteLogging *logger )
{
    uint32_t *worker = (logger) ? &logger->level:&global_level;

    return ((*worker & global_level) & test) ? 1:0;
}
//


//
//
void           remoteLogging::setLevel ( uint32_t value, remoteLogging *logger )
{
    uint32_t *worker = (logger) ? &logger->level:&global_level;

    //
    switch ( getLevel (value, logger) )
    {
        case 0  : *worker |=   value ; break;
        default : *worker &= ~(value); break;
    }
}
//


//
//
remoteLogging:: remoteLogging ( uint64_t owner, const char *category, uint32_t level, uint32_t options ) : level(level), actual_options(options)
{
    


#if 0
    //
    memset(&address, 0, sizeof(address));

    //
	struct addrinfo hints, *servinfo;
	int rv;

    //
	memset(&hints, 0, sizeof hints);
	hints.ai_family   = family; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_DGRAM;

    //
	if ( (rv = getaddrinfo( hostname ? hostname:LOCALHOST , NULL , &hints , &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

    // Filling server information
    memcpy (&address.servaddr, servinfo, ((struct sockaddr_in *) servinfo->ai_addr)->sin_len);
    address.servaddr.sin_port = htons ( (port) ? port:REMOTELOGGING_PORT );

    //
    freeaddrinfo(servinfo); // all done with this structure


    //
    if ( (address.sckt = socket(family, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(0);
    }

    //
    int sndbuf_size = 16384; //
    if (setsockopt(address.sckt , SOL_SOCKET, SO_SNDBUF, &sndbuf_size, sizeof(sndbuf_size)) == -1)
    {
        secure ();
    }
#endif

}
//


//
//
remoteLogging::~remoteLogging ( void )
{
    secure(); 

}
//


//
//
void           remoteLogging::message ( const char *file, uint64_t line, function_format func, const char *function, uint32_t level, char *category, const char prologue, const char epilogue, const char *format, ... )
{
    logger_payload payload;

    //
    memset (&payload, 0x0, sizeof (payload));

    va_list argList;
    va_start (argList, format);

    //
    payload.type     = 0;
    payload.level    = level;
    payload.depth    = 0; // depth;
    payload.line     = line;
    payload.prologue = (prologue == '\0') ? ' ':prologue;
    payload.epilogue = epilogue;

    snprintf  (payload.thread  , sizeof (logger_payload::thread  ), "mainThread" );
    if (!pthread_main_np())
        pthread_getname_np (pthread_self(), payload.thread, sizeof (logger_payload::thread  ));


    gettimeofday(&payload.timestamp, NULL);


    snprintf  (payload.file    , sizeof (logger_payload::file    ), "%s", file    );
    snprintf  (payload.function, sizeof (logger_payload::function), "%s", function);
    snprintf  (payload.category, sizeof (logger_payload::category), "%s", (category) ? category:NULL);

    if (func)
        (func) (&payload, function);

    //
    if (format)
        vsnprintf (payload.message, sizeof (logger_payload::message), format, argList);

    //
    va_end   (argList);

    //
    enqueue ( &payload );
}
//
