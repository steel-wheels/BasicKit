/*
 * @file UTArrayValue.c
 * @description The unit test for CNArrayValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTArrayValue.h"
#include "UTUtils.h"

bool
UTArrayValue(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Array value test\n", __func__) ;

        struct CNArrayValue * arr0 = CNAllocateArrayValue(vpool, 16) ;
        dumpMemoryUsage(vpool) ;
        CNReleaseValue(vpool, CNSuperClassOfArrayValue(arr0)) ;

        CNInterface()->printf("(%s) Released state\n", __func__) ;
        return checkMemoryUsage(vpool) ;
}
