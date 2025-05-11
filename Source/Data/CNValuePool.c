/*
 * @file CNValuePool.c
 * @description Define CNValuePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import "CNUtils.h"

struct CNMemoryUsage
CNMemoryUsageOfValuePool(const struct CNValuePool * src)
{
        struct CNMemoryUsage susage = CNMemoryUsageOfScalarPool(&(src->scalarPool)) ;
        struct CNMemoryUsage ausage = CNMemoryUsageOfArrayPool(&(src->arrayPool)) ;
        struct CNMemoryUsage result = {
                .allocatedSize  = susage.allocatedSize + ausage.allocatedSize,
                .usableSize     = susage.usableSize    + ausage.usableSize
        } ;
        return result ;
}

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src)
{
        CNPrintIndent(0) ; CNInterface()->printf("ValuePool:\n") ;
        CNDumpScalarPool(indent+1, &(src->scalarPool)) ;
        CNDumpArrayPool(indent+1, &(src->arrayPool)) ;
}

