//
//  AppDelegate.m
//  bedrock
//
//  Created by Alexandra Beebe on 7/28/22.
//


//
#import "bedrock.h"
#import "pluginManager.h"


//
//
int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}


//
//
void    *worker_thread ( callback_plugin *callback )
{
    (*callback) ();

    //
    pthread_exit (NULL);
}
//


//
@interface bedrock ()
{
    //
    IBOutlet NSWindow            *splash;
    IBOutlet NSProgressIndicator *ctrlprogress;
    IBOutlet NSTextField         *ctrlProgressText;
}

//
#pragma mark private bedrock api
-(uint64_t) mainThread   :(SEL) selector :(id) object;
-(id      ) createWindow :(NSViewController *) ctrler;

@end

@implementation bedrock

//
//
+(void) initialize
{
    if (self == [bedrock class])
    {
        NSLog (@"%s : %@", __FUNCTION__, self);
    }
}
//


//
#pragma mark public

//
//
-(id) init
{
    //
    if ((self = [super init]))
    {
        //
        NSLog (@"%s : %@", __FUNCTION__, self);
    }

    //
    return self;
}
//



// Really!  Needs to be here...
-(void) terminate: (id)sender
{
    [super terminate:sender];
}



//
//
-(void) finishLaunching
{
    //
    [super finishLaunching];


    [[pluginManager factory] performSelectorInBackground:@selector(bootstrap:) withObject:self];
}
//


//
//
-(void) actionMenu:(NSMenuItem *) sender
{
    callback_plugin *callback = NULL;
    if (!(callback = (callback_plugin *) sender.tag))
        return;

    //
    pthread_t boot_thread;
    pthread_create (&boot_thread, NULL, ((void *(*)(void *)) worker_thread), callback);

//
}
//



#pragma mark private bedrock api

//
//
-(uint64_t) mainThread   :(SEL) selector :(id) object;
{
    if (![NSThread isMainThread])
    {
        [self performSelectorOnMainThread:selector withObject:object waitUntilDone:NO];
        return 0;
    }

    return 1;
}
//


//
//
-(id  )     createWindow :(NSViewController *)ctrler
{
//
    NSLog (@"%s", __FUNCTION__);

//    const NSWindowStyleMask styleMask = NSWindowStyleMaskBorderless | NSWindowStyleMaskFullSizeContentView;
    const NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    NSWindow *child;
    if (!(child = [[NSWindow alloc] initWithContentRect:ctrler.view.frame styleMask:styleMask backing:NSBackingStoreBuffered defer:NO]))
        return NULL;

    //
    child.contentView     = ctrler.view;
    child.delegate        = self;

    //
    [child makeKeyAndOrderFront:self];

    //
    return child;
}
//


//
#pragma mark public bedrock api

//
//
-(void) secureSplash
{
    if (![self mainThread :_cmd :NULL])
        return;

    [splash orderOut:self];
}
//


//
//
-(void) secureProgress
{
    if (![self mainThread :_cmd :NULL])
        return;

    ctrlprogress    .hidden = 1;
    ctrlProgressText.hidden = 1;
}
//

//
//
-(void) enableProgress :(NSNumber *) count
{
    if (![self mainThread :_cmd :count])
        return;

    //
    if (ctrlprogress)
    {
        ctrlprogress.doubleValue = 0.0;
        ctrlprogress.minValue    = 0.0;
        ctrlprogress.maxValue    = [count doubleValue] + 1.0;

        //
        ctrlprogress.hidden      = FALSE;
    }

    //
    if (ctrlProgressText)
    {
        ctrlProgressText.stringValue = @"";
        ctrlProgressText.hidden      = FALSE;
    }
}
//


//
//
-(void) stepProgress   :(NSString *) message
{
    if (![self mainThread :_cmd :message])
        return;

    //
    if (ctrlprogress)
        ctrlprogress.doubleValue += 1.0;

    //
    if (ctrlProgressText)
        ctrlProgressText.stringValue = message;
}
//



//
//
-(id  ) addView        : (const char *) nibName :(NSBundle *) bundle
{
    if ( ![NSThread isMainThread] )
    {
        __block id rtn;
        dispatch_sync (dispatch_get_main_queue(), ^{
            rtn = [ self addView :nibName :bundle];
        });

        return rtn;
    }

    //
    NSViewController *ctrler;
    if ((ctrler = [[NSViewController alloc] initWithNibName:[NSString stringWithUTF8String:nibName] bundle:bundle]))
    {
        NSLog (@"%@", ctrler);
        NSLog (@"%@", ctrler.view);

//        //
//        if ([ctrler.view respondsToSelector:@selector (bootstrap)])
//            [ctrler.view performSelector:@selector(bootstrap) withObject:NULL];

        //
        [self createWindow:ctrler];
    }

//
//    NSNib *nib;
//    if (!(nib = [[NSNib alloc] initWithNibNamed:[NSString stringWithUTF8String:nibName] bundle:bundle]))
//        return NULL;
//
//    //
//    NSArray *objects;
//    if (![nib instantiateWithOwner:NULL topLevelObjects:&objects])
//        return NULL;
//
//    //
//    NSViewController *ctrler = NULL;
//
//    for (id object in objects)
//    {
//        if (![object isKindOfClass:[NSViewController class]])
//            continue;
//
//        //
//        ctrler = (NSViewController *) object;
//        if (ctrler.view == NULL)
//        {
//            ctrler = NULL;
//            continue;
//        }
//
//        [self createWindow:ctrler];
//        break;
//    }
//
    return ctrler;
}
//


//
//
-(NSMenu *) accessMenu
{
    //
    if ( ![NSThread isMainThread] )
    {
        __block NSMenu *menu;
        dispatch_sync (dispatch_get_main_queue(), ^{
            menu = [ self accessMenu ];
        });

        return menu;
    }

    if (!self.mainMenu)
        self.mainMenu = [[NSMenu alloc] initWithTitle:@"Bedrock"];

    return self.mainMenu;
}
//


//
//
-(bedrockProject *) addProject :(const char *)projectName :(const char *) projectRoot
{
    return new bedrockProject (projectName, projectRoot);
}
//

#pragma mark NSWindowDelegate

-(BOOL)windowShouldClose :(NSWindow *)sender
{
    NSLog (@"%s : %@", __PRETTY_FUNCTION__, sender);

    //
    sender.contentView    = NULL;
    sender.delegate       = NULL;

    //
    [sender orderOut:self];

    //
    [sender class];

    return 0;
}


@end





