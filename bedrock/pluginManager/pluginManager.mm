//
//  pluginManager.m
//  bedrock
//
//  Created by Alexandra Beebe on 8/10/21.
//  Copyright © 2021 Alexandra Beebe. All rights reserved.
//

//
#import "bedrock.h"
#import "pluginManager.h"

//
static pluginManager *singleton = NULL;


//
@implementation pluginManager
{
}

//
//
+(void) initialize
{
    if (self == [pluginManager class])
    {
        NSLog (@"%s : %@", __FUNCTION__, self);
    }
}
//


//
//
+(id) factory
{
    if (singleton == NULL)
        [[pluginManager alloc] init];

    //
    return singleton;
}
//



#pragma mark public

//
//
-(id  ) init
{
    //
    self = (singleton) ? singleton:self;

    //
    if (singleton == NULL)
    {
        if ((self = [super init]))
        {
            //
            singleton = self;

            //
            NSLog (@"%s : %@", __FUNCTION__, self);
        }
    }

    //
    return self;
}
//


//
//
-(void) bootstrap
{
    //
//    bedrock  *owner = (bedrock *) [NSApplication sharedApplication];

    NSString *type  = @"bedrock";
    NSString *path  = [[NSBundle mainBundle] builtInPlugInsPath];

    //
    for (NSString *object in [NSBundle pathsForResourcesOfType:type inDirectory:path])
    {
        NSLog (@"\t%@", object);

        //
        NSBundle *bundle;
        if (!(bundle = [NSBundle bundleWithPath:object]))
            continue;

        //
        if (![bundle load])
            continue;

//        Class cls;
//        if (!(cls = [bundle principalClass]))
//            continue;
//
//        //
//        NSLog (@"\t\t%@ [ %p ]", [bundle principalClass], cls);
//
//        NSObject *child;
//        if (!(child = [cls new]))
//            continue;
////        if (!(child = [[cls alloc] init:owner]))
////            continue;
//
//        NSLog (@"\t\t%@", child);

        //
        NSNib *nib;
        if ((nib = [[NSNib alloc] initWithNibNamed:[bundle objectForInfoDictionaryKey:@"NSMainNibFile"]  bundle:bundle]))
        {
            NSLog (@"\t\t%@", nib);

            //
            NSArray *objects;
            [nib instantiateWithOwner:NULL topLevelObjects:&objects];

            for (NSObject *co in objects)
            {
//                if ([co isKindOfClass:[NSWindow class]])
//                {
//                    NSWindow *window;
//                    window = (NSWindow *) co;
//
//                    NSLog (@"%@", [owner mainWindow]);
//
//                    [window makeMainWindow];
//                    [window setIsVisible:true];
//                    [window makeKeyAndOrderFront:self];
//
//                    NSLog (@"%@", [owner mainWindow]);
//                    continue;
//                }

                if ([co isKindOfClass:[NSMenu class]])
                {
                    NSLog (@"MENU   : %@", co);
                }

            }
        }


//        if ((nib = [NSNib initWithNibNamed:bundle:bundle]))

        //
//        [enumerateKeysAndObjectsUsingBlock: ^(NSString *key, NSString *obj, BOOL *stop)
//            {
//                NSLog (@"%@ : %@ [ %p ]", key, obj, NSSelectorFromString(obj));
//
//                [owner registerMenu :NULL :NULL : NSSelectorFromString(obj)];
//            }
//        ];


//        //
//        NSBundle *bundle;
//        if (!(bundle = [NSBundle bundleWithPath:object]))
//            continue;
//
//        //
//        if (![bundle load])
//            continue;
//
//        //
//        void *handle;
//        if (!(handle = dlopen( [[bundle executablePath] UTF8String], RTLD_LAZY)))
//            continue;
//
//        //
//        [self bundleLoad:handle :bundle];
//
//        //
//        dlclose( handle );
    }
}
//


@end
