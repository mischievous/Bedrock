//
//  pluginManager.h
//  bedrock
//
//  Created by Alexandra Beebe on 8/10/21.
//  Copyright © 2021 Alexandra Beebe. All rights reserved.
//

//
#import <Foundation/Foundation.h>

//
#import "api.h"


//
@interface pluginManager : NSObject
{
}

//
+(id  )     factory;

//
-(void)     bootstrap :(bedrock *) owner;



@end
