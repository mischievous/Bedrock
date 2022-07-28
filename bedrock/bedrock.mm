//
//  AppDelegate.m
//  bedrock
//
//  Created by Alexandra Beebe on 7/28/22.
//


//
#import "bedrock.h"


int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}


//

@interface bedrock ()
{
}

//
@property (strong) IBOutlet NSWindow *window;

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


//
// Really!  Needs to be here...
-(void) terminate: (id)sender
{
    [super terminate:sender];
}
//


//
//
-(void) finishLaunching
{
    // initialize the drag code...
    [super finishLaunching];
}
//

@end





