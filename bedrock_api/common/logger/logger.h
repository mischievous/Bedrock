//
//  logger_cpp.h
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/15/22.
//

#ifndef LOGGER_HEADER
#define LOGGER_HEADER

//
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

//
typedef unsigned char uint08_t;


//
#include <sys/time.h>
#include <sys/syslimits.h> // PATH_MAX
#include <functional>
#include <map>

//
#define REMOTELOGGING_PORT (48830)
#define LOCALHOST          ( "127.0.0.1" )
#define BROADCAST          ( "224.0.0.1" )

typedef enum
{
    LEVEL_OFF    = 0x00000000,
    LEVEL_NSLOG  =  (1 <<  0),
    LEVEL_SPARE1 =  (1 <<  4),
    LEVEL_TRACE  =  (1 <<  8),
    LEVEL_DEBUG  =  (1 << 12),
    LEVEL_INFO   =  (1 << 16),
    LEVEL_WARN   =  (1 << 20),
    LEVEL_ERROR  =  (1 << 24),
    LEVEL_FATAL  =  (1 << 28),

    LEVEL_ALL    = 0xffffffff,

} logger_level;


typedef enum
{
	kLoggerOption_LogToConsole						= 0x01,
//	kLoggerOption_BufferLogsUntilConnection			= 0x02,
//	kLoggerOption_BrowseBonjour						= 0x04,
//	kLoggerOption_BrowseOnlyLocalDomain				= 0x08,
//	kLoggerOption_UseSSL							= 0x10,
	kLoggerOption_CaptureSystemConsole				= 0x20,
//	kLoggerOption_BrowsePeerToPeer					= 0x40

    OPTIONS_ALL = 0xffffffff,

} logger_options;


//
// log -> stdout/stderr
// log -> file name
// log -> udp
// log -> tcp
// log -> bonjour (udp/tcp)

//
//
#pragma pack(1)
typedef struct udp_address
{
    socklen_t length;

    //
    union
    {
        char   address[SOCK_MAXADDRLEN + 1];
        struct sockaddr_in servaddr;
    };

    //
    uint16_t sckt; // socket

} udp_address;
#pragma pack()

//
//
#pragma pack(1)

typedef struct
{
    uint32_t type;

    union
    {
        uint08_t     raw      [4092];

        struct
        {
            struct timeval timestamp;

            //
            uint32_t       depth;
            char           prologue;
            char           epilogue;

            char           thread     [     256];

            char           file       [PATH_MAX];
            uint64_t       line;

            uint32_t       functionType;
            char           classname  [      46];
            char           function   [      46];

            uint32_t       level;
            char           category   [      50];
            char           message    [    2620];
        };
    };

} logger_payload; // entire message 4K
#pragma pack()


typedef std::function <void (logger_payload *, const char *function)> function_format;

//
class remoteLogging;
typedef std::map< uint64_t, remoteLogging *> m_loggers;


//
class remoteLogging
{
    private   :
//        static pthread_t       worker;
//        static pthread_mutex_t mutex;
//        static sem_t          *semaphore;

        //
        static m_loggers       loggers;
        static udp_address     address;


        //
        static uint32_t        global_level;
        static uint32_t        global_options;
        static uint32_t        depth; 

        //
        uint32_t               level;
        uint32_t               actual_options;


    protected :
    public    :


    private   :
        static   void           bootstrap     ( void );
        static   void          *worker        ( void );
        static   void           enqueue       ( logger_payload *payload );


                                remoteLogging ( uint64_t owner, const char *category, uint32_t level = LEVEL_ALL, uint32_t options = OPTIONS_ALL );
                 void           secure        ( void ) { if (!address.sckt) return; close(address.sckt);  shutdown(address.sckt, SHUT_RDWR); address.sckt = 0; }

        //
//                 uint64_t       level         ( uint32_t test ) { return (working_level & test) ? 1:0; }
//                 void           worker        ( const char *file, uint64_t line, function_format func, const char *function, uint32_t level, char *category, const char *format, va_list *argList );

    protected :

    public    :
        static   remoteLogging *logger        ( const char *file = NULL );
        static   remoteLogging *factory       ( const char *file, const char *category, uint32_t level = LEVEL_ALL, uint32_t options = OPTIONS_ALL );

        //
        static   void           cpp           ( logger_payload *, const char *function );
        static   void           objc          ( logger_payload *, const char *function );

        //
        static   uint64_t       getLevel      ( uint32_t test , remoteLogging * ); //  { return (global_level & test) ? 1:0; }
        static   void           setLevel      ( uint32_t level, remoteLogging * ); //  { if (global_level & level) global_level &= ~(level); else global_level |= level; }

        //
        static   void           enter         ( void ) { depth += 1; }
        static   void           leave         ( void ) { depth -= 1; }

        //
//        static   nslog                        ( const char *file = NULL );
//        static   udp                          ( const char *hostname = LOCALHOST, uint32_t port = REMOTELOGGING_PORT, uint32_t family = AF_INET, const char *file = NULL );



//        static   void           file          ( remoteLogging *logger, const char *path );
//        static   void           udp           ( remoteLogging *logger, const char *hostname = LOCALHOST, uint32_t port = REMOTELOGGING_PORT, uint32_t family = AF_INET );
//        static   void           tcp           ( remoteLogging *logger, const char *hostname = LOCALHOST, uint32_t port = REMOTELOGGING_PORT, uint32_t family = AF_INET );
//        static   void           std           ( remoteLogging *logger );


        virtual                ~remoteLogging ( void );

        //
//                 void           setLevel      ( uint32_t value ) { if (level & value) level &= ~(value); else level |= value; }
//                 uint64_t       getLevel      ( uint32_t test  ) { return ((level & global_level) & test) ? 1:0; }

        //

                 void           message       ( const char *file, uint64_t line, function_format func, const char *function, uint32_t level, char *category, const char prologue, const char epilogue, const char *format, ... );
};


#endif /* REMOTE_LOGGING_HPP_ */
