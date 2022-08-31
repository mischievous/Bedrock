//
//  bedrockProject.cpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

//
#include <string.h>

//
#include "bedrockProject.h"


//
//
               bedrockProject:: bedrockProject ( const char *path ) : _root_(NULL)
{
    memset (_name_, 0x0, sizeof(_name_));

    //
    snprintf (_name_, sizeof (_name_), "%s", strrchr (path, '/') + 1);

    //
    _root_ = addObject (PO_ROOT, _name_, path, NULL);
}
//


//
//
               bedrockProject::~bedrockProject ( void )
{

}
//

//
//
projectObject *bedrockProject:: addObject      ( uint64_t type, const char *name, const char *path, objects_t *dependancies, uint64_t uuid )
{
    projectObject *object;
    if (!(object = new projectObject ( type, name, path, dependancies )))
        return NULL;

    //
    _master_.push_back ( object );

    if (_root_)
    {
        switch (type)
        {
            case PO_TARGET : _root_->child ( object ); break;
        }
    }

    //
    if (dependancies)
    {
        for (projectObject *child : *dependancies )
            object->child (child);
    }

    return object;
}
//
