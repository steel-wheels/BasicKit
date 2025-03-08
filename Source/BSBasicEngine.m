//
//  BSBasicEngine.m
//  BasicKit
//
//  Created by Tomoo Hamada on 2025/03/08.
//

#import "BSBasicEngine.h"

@implementation BSBasicEngine

- (id) init
{
        self = [super init] ;
        mVersion = 1;
        return self ;
}

- (NSInteger) version
{
        return mVersion ;
}

@end
