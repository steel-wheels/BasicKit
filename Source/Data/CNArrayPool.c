/*
 * @file CNArrayPool.c
 * @description Define CNArrayPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNInterface.h>

void
CNDumpArrayPool(const struct CNArrayPool * src)
{;
        CNInterface()->printf("ArrayPool: elementSize=%lu\n", src->elementSize) ;
        CNDumpPagePool(&(src->pagePool)) ;
}
