//
//  json.hpp
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/6/22.
//

//
// https://www.geeksforgeeks.org/overloading-subscript-or-array-index-operator-in-c/?ref=rp


#ifndef BEDROCK_JSON
#define BEDROCK_JSON

//
#include <stdint.h>
#include <memory.h>

//
#include "json.h"
#include "json.hpp"


//
//class json_object;


//
class bedrock_json
{
    private   :

        nlohmann::json  root;
        json_object     object;

        //
        std::string     output;


    protected :
    public    :

    private   :
    protected :
    public    :
        //
        virtual           ~bedrock_json ( void ) {;}

        //
                           bedrock_json ( void ) : root (nlohmann::json::value_t::object)  {;}

        //
        nlohmann::json    *json         ( void )                      { return &root; }


        //
        void               load         ( FILE               *value ) { root   = nlohmann::json::parse(value); }
        const char        *save         ( void                      ) { output = root.dump(1, ' '); return output.c_str(); }

        //
        uint64_t           contains     (       char         *path  ) { return contains ((const char *) path); }
        uint64_t           contains     ( const char         *path  ) { object.json ( &root ); return object.contains (path); }

        //
        uint64_t           remove       (       char         *path  ) { return remove ((const char *) path); }
        uint64_t           remove       ( const char         *path  ) { object.json ( &root ); return object.remove (path); }

        // accessors
        json_object       &operator[]   (       char         *path  ) { return operator[] ((const char        *) path); }
        json_object       &operator[]   ( const char         *path  ) { object.json ( &root ); return object[path]; }
        json_object       &operator[]   (       uint64_t      path  ) { object.json ( &root ); return object[path]; }
};

#endif /* json_hpp */
