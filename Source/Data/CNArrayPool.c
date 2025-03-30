/*
 * @file CNArrayPool.c
 * @description Define CNArrayPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNUtils.h>
#include <stdio.h>

void
CNDumpArrayPool(unsigned int indent, const struct CNArrayPool * src)
{
        CNDumpIndent(indent) ;
        printf("ArrayPool: elementSize=%lu\n", src->elementSize) ;
        CNDumpPagePool(indent+1, &(src->pagePool)) ;
}
