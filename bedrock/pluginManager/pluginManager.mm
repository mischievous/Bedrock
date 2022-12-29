//
//  pluginManager.m
//  bedrock
//
//  Created by Alexandra Beebe on 8/10/21.
//  Copyright © 2021 Alexandra Beebe. All rights reserved.
//

//
#import <map>
#import <dlfcn.h>

//
#import "log.h"

//
#import "pluginManager.h"

//
#define FACTORY         ("_ZN%lu%s7factoryEP8NSBundlePv")
#define PRINCIPAL_CLASS (@"bedrockClass")

//
typedef std::map <uint128_t, void *> plugins_m;

//
static pluginManager *singleton = NULL;


//
//
@interface pluginManager ()
{
    plugins_m plugins;
}

//
-(void) load :(NSBundle *) bundle :(bedrock  *) owner :(void *) handle :(NSString *) className;

//
-(void  )   addPlugin  :(uint128_t) key :(void *) symbol;
-(void *)   getPlugin  :(uint128_t) key;


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
        log_void ();
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


//
//
+(void *) loadSymbol :(void *) library :(const char *) symbol :(const char *) className
{
    size_t length = strlen (className);

    char symbolName[256];
    snprintf (symbolName, sizeof (symbolName), symbol, length, className);


    return dlsym (library, symbolName);
}
//


//
//
+(void  ) addPlugin:(uint128_t)key :(void *)symbol
{
    return [singleton addPlugin:key :symbol];
}
//


//
//
+(void *) getPlugin:(uint128_t)key
{
    return [singleton getPlugin:key];
}
//


#pragma mark public

//
//
-(id    ) init
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
            log_self();
        }
    }

    //
    return self;
}
//

//
//
-(void  ) load :(NSBundle *) bundle :(bedrock  *) owner :(void *) handle :(NSString *) className
{
    //
    pluginFactory_t factory;
    if (!(factory = (pluginFactory_t) [pluginManager loadSymbol:handle :FACTORY :[className UTF8String]]))
        return;

    //
    bedrockPlugin *plugin;
    plugin = (bedrockPlugin *) factory ( bundle, handle );
}
//


//
//
-(void  ) bootstrap :(bedrock  *) owner
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
}
//


//
//
-(void  ) addPlugin:(uint128_t)key :(void *)symbol
{
    //
    plugins[key] = symbol;
}
//


//
//
-(void *) getPlugin:(uint128_t)key
{
    void *plugin = NULL;
    if (plugins.find(key) != plugins.end())
        plugin = plugins[key];

    return plugin;
}
//
@end
