//
//  projectObject.cpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

//
#include <algorithm>

//
#include "projectObject.h"
#include "bedrockProject.h"

//
//
uint64_t vector_find (uint128_t key, objects_t *vector)
{
    //
    for (uint128_t target : *vector)
    {
        if (key == target)
            return 1;
    }

    return 0;
}
//

//
//
projectObject:: projectObject ( uint64_t type, const char *path, const char *name, bedrockProject *ownr )
{
    //
    object.uuid.actual = uuid_generate();

    //
    parent           = NULL;
    owner            = ownr;

    //
    object.type      = type;

    //
    memset   (object.name, 0x0, sizeof (objectData::name));
    memset   (object.path, 0x0, sizeof (objectData::path));

    //
    snprintf (object.path, sizeof (objectData::path), "%s", path);
    snprintf (object.name, sizeof (objectData::name), "%s", name);
}
//


//
//
projectObject::~projectObject ( void )
{

}
//


//
//
bool     projectObject::compare ( projectObject *lhs, projectObject *rhs )
{
    return (strcasecmp ( lhs->object.name, rhs->object.name ) <= 0) ? 1:0;
}
//

//
//
void     projectObject::sort      ( void )
{
//FIXME
#if 0
    if (children.size() == 0)
        return;

    for (uint32_t a = 0; a < children.size() - 1; a+= 1)
    {

        uint128_t      ldx = children[a];
        projectObject *lhs;
        if (!(lhs = owner->object ( ldx )))
            continue;

        //
        for (uint32_t b = a + 1; b < children.size(); b += 1)
        {
            uint128_t      rdx = children[b];
            projectObject *rhs;
            if (!(rhs = owner->object ( rdx )))
                continue;


            if (compare ( lhs, rhs ) == 1)
                continue;

            //
            rdx ^= ldx; ldx ^= rdx; rdx ^= ldx;

            //
            children.at ( a ) = ldx;
            children.at ( b ) = rdx;

            //
            lhs = owner->object ( ldx );
            rhs = owner->object ( rdx );
        }
    }
#endif
}
//

//
//
char    *projectObject::path      ( void )
{
    return object.path;
}
//


//
//
void           projectObject::dependancy ( projectObject *object )
{
    //
    if (!object)
        return;

    //
    object->parent = this;

    //
    uint128_t uuid = object->uuid().actual;

    //
    if (!(vector_find ( uuid, &dependancies)))
        dependancies.push_back(uuid);

    //
    switch (object->type())
    {
        case PO_SOURCE : child ( object ); break;
        default        : break; // ignore it.
    }
}
//


//
//
projectObject *projectObject::child ( uint64_t idx )
{
// FIXME
#if 0
    if (idx >= children.size())
        return NULL;

    //
    return owner->object ( children[idx] );
#endif
return NULL;
}
//


//
//
void           projectObject::child      ( projectObject *object )
{
    //
    if (!object)
        return;

    //
    object->parent = this;

    //
    uint128_t uuid = object->uuid().actual;

    //
    if (!(vector_find ( uuid, &children)))
        children.push_back(uuid);
}
//
