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
}

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
    
    //
    [[pluginManager factory] bootstrap];
}
//


//
#pragma mark bedrock api

//
//
//-(void) init_nib :(id) self :(NSString *) nibname
//{
//}
//


@end





