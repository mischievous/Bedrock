//
//  bedrockProject.cpp
//  bedrock
//
//  Created by Alexandra Beebe on 8/1/22.
//

#include "bedrockProject.h"


//
//
               bedrockProject:: bedrockProject ( const char *path ) : _root_(NULL)
{
    _root_ = addObject (PO_ROOT, "NAME", NULL);
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
projectObject *bedrockProject:: addObject      ( uint64_t type, const char *name, objects_t *dependancies )
{
    projectObject *object;
    if (!(object = new projectObject ( type, name, dependancies )))
        return NULL;

    if (_root_)
        _root_->child ( object );

//    switch (type)
//    {
//        case PO_TARGET : targets.push_back (object); break;
//    }

    return object;
}
//
