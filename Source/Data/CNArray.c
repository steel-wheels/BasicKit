/*
 * @file CNArray.c
 * @description Define CNArray data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArray.h>
#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNValue.h>

void
CNArrayDump(unsigned int indent, uint32_t count, const struct CNArray * src)
{
        const struct CNValue * values = src->values ;
        const struct CNValue * endval = values + count ;
        for( ; values < endval ; values++){
                CNValueDump(indent + 1, values) ;
        }
}
