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
+(void *)   loadSymbol :(void *) library :(const char *) symbol :(const char *) className;

// library plugins
+(void  )   addPlugin  :(uint128_t) key :(void *) symbol;
+(void *)   getPlugin  :(uint128_t) key;

//
-(void)     bootstrap  :(bedrock *) owner;

@end
