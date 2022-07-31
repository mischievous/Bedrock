//
//  AppDelegate.h
//  bedrock
//
//  Created by Alexandra Beebe on 7/28/22.
//

#import <Cocoa/Cocoa.h>

@interface bedrock : NSApplication
{
}

//
-(void    ) actionMenu :(NSMenuItem *) sender;

#pragma mark public bedrock api

// splash
-(void    ) secureSplash;

// status 
-(void    ) secureProgress;
-(void    ) enableProgress :(NSNumber *) count;
-(void    ) stepProgress   :(NSString *) message;

// window/view
-(id      ) addView        :(NSNib    *) nib;

// menu
-(NSMenu *) accessMenu;

@end

