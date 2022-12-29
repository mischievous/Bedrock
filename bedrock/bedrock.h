//
//  AppDelegate.h
//  bedrock
//
//  Created by Alexandra Beebe on 7/28/22.
//

//
#import <map>

//
#import <Cocoa/Cocoa.h>

//
#import "bedrockProject.h"


//
@interface bedrock : NSApplication <NSWindowDelegate>
{
}

//
-(void            ) actionMenu :(NSMenuItem *) sender;

#pragma mark public bedrock api

// splash
-(void            ) secureSplash;

// status 
-(void            ) secureProgress;
-(void            ) enableProgress :(NSNumber *) count;
-(void            ) stepProgress   :(NSString *) message;

// window/view
-(id              ) addWindow      :(const char *) nibName    :(const char *) bundle :(NSWindowStyleMask) styleMask;
-(id              ) addView        :(const char *) nibName    :(NSBundle *) bundle;

// menu
-(NSMenu         *) accessMenu;

// projects
-(bedrockProject *) addProject     :(const char *) projectName :(const char *) projectRoot;


@end

