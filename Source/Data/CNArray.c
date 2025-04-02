/*
 * @file CNArray.c
 * @description Define CNArray data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArray.h>
#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNUtils.h>
#include <stdio.h>

void
CNRetainArray(struct CNArray * dst)
{
        struct CNValue * values = dst->values ;
        uint32_t        count  = dst->count ;

        struct CNValue * ptr    = values ;
        struct CNValue * endptr = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                CNRetainValue(values) ;
        }
}

void
CNReleaseArray(struct CNValuePool * pool, struct CNArray * dst)
{
        uint32_t        count  = dst->count ;
        struct CNValue * values = dst->values ;
        struct CNValue * ptr    = values ;
        struct CNValue * endptr = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                CNReleaseValue(pool, ptr) ;
        }
        CNFreeArrayData(&(pool->arrayPool), count, values) ;
}

void
CNArrayDump(unsigned int indent, uint32_t count, const struct CNArray * src)
{
        const struct CNValue * values  = src->values ;
        const struct CNValue * endval  = values + count ;
        for( ; values < endval ; values++){
                CNDumpValue(indent + 1, values) ;
        }
}
