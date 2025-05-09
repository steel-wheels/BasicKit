/*
 * @file UTUtils.c
 * @description The utility functiosn for test
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTUtils.h"

void
dumpMemoryUsage(struct CNValuePool * vpool)
{
        struct CNMemoryUsage usage = CNMemoryUsageOfValuePool(vpool) ;
        CNPrintMemoryUsage(&usage) ;
}

bool
checkMemoryUsage(struct CNValuePool * vpool)
{
        bool result ;
        struct CNMemoryUsage usage = CNMemoryUsageOfValuePool(vpool) ;
        CNPrintMemoryUsage(&usage) ;
        if(usage.allocatedSize == usage.usableSize) {
                CNInterface()->printf("(%s) No memory leak\n", __func__) ;
                result = true ;
        } else {
                CNInterface()->printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }
        return result ;
}


