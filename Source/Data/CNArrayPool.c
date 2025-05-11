/*
 * @file CNArrayPool.c
 * @description Define CNArrayPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNInterface.h>
#import "CNUtils.h"

void
CNDumpArrayPool(unsigned int indent, const struct CNArrayPool * src)
{
        CNPrintIndent(indent) ; CNInterface()->printf("ArrayPool\n") ;
        CNDumpPagePool(indent+1, &(src->pagePool)) ;
}
