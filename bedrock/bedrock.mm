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
-(void    ) createWindow :(NSView *) contentView;

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
    callback_plugin *callback;
    if (!(callback = (callback_plugin *) sender.tag))
        (*callback) ();
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
-(void)     createWindow :(NSView *)contentView
{
//    const NSWindowStyleMask styleMask = NSWindowStyleMaskBorderless | NSWindowStyleMaskFullSizeContentView;
    const NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    NSWindow *child;
    if (!(child = [[NSWindow alloc] initWithContentRect:contentView.frame styleMask:styleMask backing:NSBackingStoreBuffered defer:NO]))
        return;

    //
    child.contentView     = contentView;

    //
    child.title           = @"Project";

    //
    [child makeKeyAndOrderFront:self];
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
-(id  ) addView        :(NSNib *)nib
{
    //
    if (!nib)
        return NULL;

    if ( ![NSThread isMainThread] )
    {
        __block id rtn;
        dispatch_sync (dispatch_get_main_queue(), ^{
            rtn = [ self addView:nib ];
        });

        return rtn;
    }

    //
    NSArray *objects;
    if (![nib instantiateWithOwner:NULL topLevelObjects:&objects])
        return NULL;

    //
    NSViewController *ctrler = NULL;

    for (id object in objects)
    {
        if (![object isKindOfClass:[NSViewController class]])
            continue;

        //
        ctrler = (NSViewController *) object;
        if (ctrler.view == NULL)
        {
            ctrler = NULL;
            continue;
        }

        [self createWindow:ctrler.view];
    }

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

@end





