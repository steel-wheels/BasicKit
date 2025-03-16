/*
 * @file BSBasicEngine.h
 * @description Define BSBasicEngine class
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <Foundation/Foundation.h>

@interface BSBasicEngine : NSObject
{
        NSInteger mVersion ;
}

- (id) init ;
- (void) dealloc ;
- (NSInteger) version ;

@end

