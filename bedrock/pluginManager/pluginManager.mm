//
//  pluginManager.m
//  bedrock
//
//  Created by Alexandra Beebe on 8/10/21.
//  Copyright © 2021 Alexandra Beebe. All rights reserved.
//

//
#import <dlfcn.h>

//
#import "pluginManager.h"

//
#define FACTORY         (@"_ZN%lu%@%lu%@FactoryEP8NSBundleP7bedrock")
//#define PRINCIPAL_CLASS (@"NSPrincipalClass")
#define PRINCIPAL_CLASS (@"bedrockClass")

//
static pluginManager *singleton = NULL;


//
//
@interface pluginManager ()
{
    // Fucking APPLE!
//    NSMutableArray *bundles;

}

//
-(void) load :(NSBundle *) bundle :(bedrock  *) owner :(void *) handle :(NSString *) className;


@end

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
//            bundles   = [NSMutableArray arrayWithCapacity:0];

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
-(void) load :(NSBundle *) bundle :(bedrock  *) owner :(void *) handle :(NSString *) className
{
    //
    NSString *factoryName;
    factoryName = [NSString stringWithFormat:FACTORY, className.length, className, className.length + 7, className];

    //
    pluginFactory_t factory;
    if (!(factory = (pluginFactory_t) dlsym ( handle, [factoryName UTF8String] )))
        return;

    //
    bedrockPlugin *plugin;
    plugin = (bedrockPlugin *) factory ( bundle, owner );
}
//


//
//
-(void) bootstrap :(bedrock  *) owner 
{
    //
    if ([NSThread isMainThread])
        return;

    //
    NSString *type    = @"bedrock";
    NSString *path    = [[NSBundle mainBundle] builtInPlugInsPath];

    //
    NSArray  *plugins =  [NSBundle pathsForResourcesOfType:type inDirectory:path];

    //
    [owner enableProgress :[NSNumber numberWithDouble:(double) plugins.count]];

    //
    for (NSString *plugin in plugins)
    {
        [owner stepProgress :[[plugin lastPathComponent] stringByDeletingPathExtension]];

        //
        NSBundle *bundle;
        if (!(bundle = [NSBundle bundleWithPath:plugin]))
            continue;

        //
        if (![bundle load])
            continue;

        //
        void *handle;
        if (!(handle = dlopen( [[bundle executablePath] UTF8String], RTLD_LAZY)))
            continue;

        //
        [self load :bundle :owner :handle :[bundle infoDictionary][PRINCIPAL_CLASS]];

        //
        dlclose( handle );

        //
//        [bundles addObject:bundle];
sleep (1);
    }


    //
    [owner secureProgress];
sleep (1);

    //
    [owner secureSplash];


//NSLog (@"%@", bundles);
}
//


@end
