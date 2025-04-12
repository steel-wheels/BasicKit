/*
 * @file CNValuePool.c
 * @description Define CNValuePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src)
{
        CNDumpIndent(indent) ; CNInterface()->printf("ValuePool\n") ;
        CNDumpScalarPool(indent + 1, &(src->scalarPool)) ;
        CNDumpArrayPool(indent  + 1, &(src->_arrayPool)) ;
}

