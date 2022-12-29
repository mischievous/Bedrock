//
//  json.h
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/8/22.
//

#ifndef JSON_OBJECT
#define JSON_OBJECT

//
#include <string>
#include <vector>
#include <map>

//
#include "json.hpp"
#include "tss.h"

//
//#include "bedrock_json.h"
class bedrock_json;


//
typedef std::map    < nlohmann::json *, std::string> t_strings;
typedef std::vector < bedrock_json   *> t_array;


//
class json_object // : public nlohmann::json
{
    private   :
        thread_specific_ptr <nlohmann::json *> object;
        t_strings                              strings;

    protected :
    public    :

    private   :

        //
                 nlohmann::json *add             ( nlohmann::json *child );

        //
                 const char     *get_string      ( void );
                 uint64_t        get_value       ( void );

    protected :
    public    :
        //
        virtual                 ~json_object     ( void );

        //
                                 json_object     ( void ) : object(NULL) { ; }

                 uint64_t        contains        (       char         *path  ) { return contains ((const char *) path); }
                 uint64_t        contains        ( const char         *path  );


                 uint64_t        remove          (       char         *path  ) { return remove ((const char *) path); }
                 uint64_t        remove          ( const char         *path  );
                 uint64_t        remove          ( uint64_t            index );

        //
                 void            json            ( nlohmann::json *value     ) { object = value; }
                 nlohmann::json *json            ( void )                      { return object.get(); }


        // accessors
                 json_object    &operator[]      (       char         *value ) { return operator[] ((const char        *) value); }
                 json_object    &operator[]      ( const char         *      );
                 json_object    &operator[]      (       uint64_t      value );
        

        // char *

                 void            operator=      (       char         *value ) {        operator=  ((const char        *) value); }
                 void            operator=      ( const char         *      );
        operator const char                    *( void                      ) { return get_string (); }

        // uint64_t
                 void            operator=      ( const uint64_t            );
        operator uint64_t                       ( void                      ) { return get_value  (); }

        // uint32_t
//        operator uint32_t                       ( void                      ) { return (uint32_t) get_value (); }
//        operator time_t                         ( void                      ) { return            get_value (); }



        // array
                 void            operator=      (       t_array      *value ) {        operator=  ((const t_array      ) *value); }
                 void            operator=      ( const t_array       value );

        // object
                 void            operator=      (       bedrock_json *value );
                 void            operator=      (       bedrock_json  value );


};


#endif /* json_h */
