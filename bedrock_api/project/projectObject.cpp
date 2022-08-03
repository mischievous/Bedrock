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

//
//
projectObject:: projectObject ( uint64_t type, const char *name, const char *path, objects_t *dependancies )
{
    this->type  = type;
    this->cell  = NULL;

    //
    memset   (this->name, 0x0, sizeof (projectObject::name));
    snprintf (this->name, sizeof (projectObject::name), "%s", name);

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
bool     projectObject::compare   ( projectObject *lhs, projectObject *rhs )
{
    return (strcmp ( lhs->name, rhs->name ) <= 0) ? 1:0;
}
//

//
//
void     projectObject::sort      ( void )
{
    std::sort(children.begin(), children.end(), projectObject::compare);
}
//
