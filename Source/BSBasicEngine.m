/*
 * @file BSBasicEngine.m
 * @description Define BSBasicEngine class
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import "BSBasicEngine.h"

@implementation BSBasicEngine

- (id) init
{
        self = [super init] ;
        mVersion = 1;
        return self ;
}

- (void) dealloc ;
{
}

- (NSInteger) version
{
        return mVersion ;
}

@end
