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
        self->mVersion = 0 ;
        return self ;
}

- (int) version
{
        return self->mVersion ;
}

@end
