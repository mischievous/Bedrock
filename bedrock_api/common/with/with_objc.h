//
//  with_objc.hpp
//  prayingMantis
//
//  Created by Alexandra Beebe on 12/8/22.
//

#ifndef WITH_OBJC
#define WITH_OBJC

//
#define with_changeValue( key, object, value ) \
    if (value != object) \
    { \
        [self willChangeValueForKey:@#key]; \
        object = value; \
        [self didChangeValueForKey :@#key]; \
    }


//
//#define with_changeValue_Action( key, object, value, action ) \
//    if (value != object) \
//    { \
//        [self willChangeValueForKey:@#key]; \
//        object = value; \
//        action (object); \
//        [self didChangeValueForKey :@#key]; \
//    }


#endif /* with_objc_hpp */
