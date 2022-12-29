//
//  log.h
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/17/22.
//

#ifndef LOG
#define LOG

//
#include "logger.h"

#ifdef DEBUG
    #define __message__( levelA, levelB, func, category, p, e, ... ) \
    { \
        remoteLogging *logger; \
        if ((logger = remoteLogging::logger(__FILE__))) \
        { \
            if (remoteLogging::getLevel ( levelA, logger )) \
            { \
                logger->message (__FILE__, __LINE__, func, __PRETTY_FUNCTION__, levelB , category, p, e, __VA_ARGS__); \
            } \
        } \
    }

    //
    #define LOGGER_DEFAULT( ... ) remoteLogging::factory ( NULL    , __VA_ARGS__)
    #define LOGGER_FACTORY( ... ) remoteLogging::factory ( __FILE__, __VA_ARGS__)
    #define LOGGER( )             remoteLogging::logger(__FILE__)


#else
    #define __message__( levelA, levelB, func, category, ... )

    #define LOGGER_DEFAULT( ... ) NULL
    #define LOGGER_FACTORY( ... ) NULL
    #define LOGGER( )             NULL


//    #define NSLog(...)           if (false) {}
//    #define trace(category, ...) if (false) {}
//    #define debug(category, ...) if (false) {}
//    #define info( category, ...) if (false) {}
//    #define warn( category, ...) if (false) {}
//    #define error(category, ...) if (false) {}
//    #define fatal(category, ...) if (false) {}

#endif


//
#define __nslog(     func, p, e, ...)                  __message__ (LEVEL_ALL  , LEVEL_NSLOG, func, (char *) "NSLog" , p, e, __VA_ARGS__)
#define __log_access(func, level, category, ...) __message__ (      level,       level, func, category, '\0', '\0', __VA_ARGS__)
#define __log_secure(func, level, category, ...) __message__ (      level,       level, func, category, '\0', '\0', __VA_ARGS__)
#define log_trace(   func, category, ...)        __message__ (LEVEL_TRACE, LEVEL_TRACE, func, category, '\0', '\0', __VA_ARGS__)
#define log_debug(   func, category, ...)        __message__ (LEVEL_DEBUG, LEVEL_DEBUG, func, category, '\0', '\0', __VA_ARGS__)
#define log_info(    func, category, ...)        __message__ (LEVEL_INFO , LEVEL_INFO , func, category, '\0', '\0', __VA_ARGS__)
#define log_warn(    func, category, ...)        __message__ (LEVEL_WARN , LEVEL_WARN , func, category, '\0', '\0', __VA_ARGS__)
#define log_error(   func, category, ...)        __message__ (LEVEL_ERROR, LEVEL_ERROR, func, category, '\0', '\0', __VA_ARGS__)
#define log_fatal(   func, category, ...)        __message__ (LEVEL_FATAL, LEVEL_FATAL, func, category, '\0', '\0', __VA_ARGS__)

//
#define WITH_TRACE(FUNC, category, ...) \
    log_access (level, category, ##__VA_ARGS__); \
    FUNC; \
    log_secure (level, category, ##__VA_ARGS__)

#define WITH_DEBUG(FUNC, category, ...) \
    log_access (level, category, ##__VA_ARGS__); \
    FUNC; \
    log_secure (level, category, ##__VA_ARGS__)

#define WITH_INFO( FUNC, category, ...) \
    log_access (level, category, ##__VA_ARGS__); \
    FUNC; \
    log_secure (level, category, ##__VA_ARGS__)


#if __OBJC__
    #define log_enter()                      remoteLogging::enter(); __nslog      (&remoteLogging::objc, '+', '\0', NULL )
    #define log_leave()                      __nslog      (&remoteLogging::objc,           '+', '\0', NULL ); remoteLogging::leave()
    #define nsLog(...)                       __nslog      (&remoteLogging::objc,           '\0', '\0', __VA_ARGS__ )

    #define log_void()                       __nslog      (&remoteLogging::objc,           '\0', '\0', NULL)
    #define log_todo()                       __nslog      (&remoteLogging::objc,           '\0', '\0', "TODO" )
    #define log_object( obj )                __nslog      (&remoteLogging::objc,           '\0', '\0', "%s", [obj  description].UTF8String )
    #define log_self()                       log_object (self)
    
    #define log_access(level, category, ...) __log_access (&remoteLogging::objc,           __VA_ARGS__ )
    #define log_secure(level, category, ...) __log_access (&remoteLogging::objc, category, __VA_ARGS__ )
    #define trace(category, ...)             log_trace    (&remoteLogging::objc, category, __VA_ARGS__ )
    #define debug(category, ...)             log_debug    (&remoteLogging::objc, category, __VA_ARGS__ )
    #define info( category, ...)             log_info     (&remoteLogging::objc, category, __VA_ARGS__ )
    #define warn( category, ...)             log_warn     (&remoteLogging::objc, category, __VA_ARGS__ )
    #define error(category, ...)             log_error    (&remoteLogging::objc, category, __VA_ARGS__ )
    #define fatal(category, ...)             log_fatal    (&remoteLogging::objc, category, __VA_ARGS__ )

#elif __cplusplus

    #define log_enter()                      remoteLogging::enter(); __nslog      (&remoteLogging::cpp, '+', '\0', NULL )
    #define log_leave()                      __nslog      (&remoteLogging::cpp,        '+', '\0', NULL ); remoteLogging::leave()
    #define nsLog(...)                       __nslog      (&remoteLogging::cpp,           '\0', '\0', __VA_ARGS__ )
    #define log_access(level, category, ...) __log_access (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define log_secure(level, category, ...) __log_access (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define trace(category, ...)             log_trace    (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define debug(category, ...)             log_debug    (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define info( category, ...)             log_info     (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define warn( category, ...)             log_warn     (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define error(category, ...)             log_error    (&remoteLogging::cpp, category, __VA_ARGS__ )
    #define fatal(category, ...)             log_fatal    (&remoteLogging::cpp, category, __VA_ARGS__ )

#else
    #define log_enter()                      remoteLogging::enter(); __nslog      (NULL,  '+', '\0', NULL )
    #define log_leave()                      __nslog      (NULL,           '+', '\0', NULL ); remoteLogging::leave()
    #define nsLog(...)                       __nslog      (NULL,           '\0', '\0', __VA_ARGS__ )

    #define log_access(level, category, ...) __log_access (NULL, category, __VA_ARGS__ )
    #define log_secure(level, category, ...) __log_access (NULL, category, __VA_ARGS__ )
    #define trace(category, ...)             log_trace    (NULL, category, __VA_ARGS__ )
    #define debug(category, ...)             log_debug    (NULL, category, __VA_ARGS__ )
    #define info( category, ...)             log_info     (NULL, category, __VA_ARGS__ )
    #define warn( category, ...)             log_warn     (NULL, category, __VA_ARGS__ )
    #define error(category, ...)             log_error    (NULL, category, __VA_ARGS__ )
    #define fatal(category, ...)             log_fatal    (NULL, category, __VA_ARGS__ )

#endif


//
//class with_logger : public with_template<remoteLogging *>
//{
//    public     :
//                 with_open ( remoteLogging *logger ) : with_template<remoteLogging *> ( logger ) { ; }
//        virtual ~with_open ( void )                                                              { close (  this->get() ); };
//};




#endif /* log_h */
