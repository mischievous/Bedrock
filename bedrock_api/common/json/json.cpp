//
//  json.cpp
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/8/22.
//

//
#include "json.h"
#include "bedrock_json.h"


//
//
json_object::~json_object ( void )
{
}
//

////
////
//json_object:: json_object ( void ) : object(NULL)
//{
//}
////

//
//
nlohmann::json *json_object::add            ( nlohmann::json    *child )
{
//    switch ( ((nlohmann::json *) object)->type() )
    switch ( object->type() )
    {
        //
        case nlohmann::json::value_t::object:
            printf ("bedrock_json::add [ PRIVATE ] illegal type\n");
            exit (2);
            break;

        case nlohmann::json::value_t::array:
            object->push_back(*child);
            break;

        default             :
            object->swap ( *child );
            child = object.get();

            break;
    }

    return child;
}
//

//
//
uint64_t        json_object::contains       ( const char        *path  )
{
    return (object->contains(path)) ? 1:0;
}
//


//
//
uint64_t        json_object::remove         ( const char        *path  )
{
    //
    if (!contains (path))
        return 0;

    //
    switch ( object->type() )
    {
        //
        case nlohmann::json::value_t::object:
            object->erase(path);
            break;

        case nlohmann::json::value_t::array:
            object->erase(path);
            break;

        default             :
            printf ("bedrock_json::remove [ PRIVATE ] illegal type\n");
            exit (2);
            break;
    }

    //
    return 1;
}
//


//
//
uint64_t        json_object::remove         ( uint64_t           idx   )
{
    if (object->type() != nlohmann::json::value_t::array)
        return 0;

    //
    if (idx >= object->size())
        return 0;

    object->erase(idx);
    
    //
    return 1;
}
//


//
// accessors
json_object    &json_object::operator[]     ( const char        *path  )
{
    //
    nlohmann::json *worker;
    if (!(worker = object.get()))
        return (json_object &) *((json_object *) this);

    //
    if (!worker->contains(path))
        (*worker)[path] = nlohmann::json::value_t::null;

    //
    object = &(*worker)[path];

    //
    return (json_object &) *((json_object *) this);
}
//


//
//
json_object    &json_object::operator[]     (       uint64_t     path  )
{
    //
    nlohmann::json *worker;
    if (!(worker = object.get()))
        return (json_object &) *((json_object *) this);


    if (!(worker->is_array()))
    {
        object = NULL;
        return (json_object &) *((json_object *) this);
    }

    //
    while (path >= worker->size())
        worker->push_back( nlohmann::json::value_t::null );

    //
    object = &(worker->at (path));

    //
    return (json_object &) *((json_object *) this);
}
//


//
// char *
void            json_object::operator=      ( const char         *value )
{
    nlohmann::json *child = new nlohmann::json (nlohmann::json::value_t::string);

    //
    (*child) = value;

    //
    add ( child );
}
//


//
//
const char     *json_object::get_string     ( void                      )
{
    //
    nlohmann::json *worker;
    if (!(worker = object.get()))
        return NULL;

    //
    if (!worker->is_string())
        return NULL;

    //
    strings[worker] = (*worker);

    //
    return strings[worker] .c_str();
}
//


//
// uint64_t
void            json_object::operator=      ( const uint64_t      value )
{
    nlohmann::json *child = new nlohmann::json (nlohmann::json::value_t::number_unsigned);

    //
    (*child) = value;

    //
    add ( child );
}
//


//
//
uint64_t        json_object::get_value      ( void                      )
{
    nlohmann::json *worker;
    if (!(worker = object.get()))
        return 0;

    uint64_t rtn = 0;
    if (worker->is_number_unsigned())
        rtn = (*worker);

    return rtn;
}
//


//
// array
void            json_object::operator=      ( const t_array      value  )
{
    nlohmann::json child = nlohmann::json::value_t::array;

    //
    object = add ( &child );

    //
    for ( bedrock_json *child : value )
    {
        nlohmann::json x = nlohmann::json(  *(child->json()) );
        add ( &x );
    }
}
//



//
// object
void            json_object::operator=      (       bedrock_json *value )
{
    add ( value->json() );
}
//



//
// object
void            json_object::operator=      (       bedrock_json  value )
{
    //
    add ( value.json() );
}

