//
//  projectObject.cpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

#include "projectObject.h"

//
//
projectObject:: projectObject ( uint64_t type, const char *name, objects_t *dependancies )
{
    this->type  = type;
    this->image = NULL;
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
