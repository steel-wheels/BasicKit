/*
 * @file CNValue.h
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUE_H
#define CNVALUE_H

#import <BasicKit/CNType.h>
#import <BasicKit/CNList.h>
#import <BasicKit/CNString.h>

struct CNValue
{
        char                    charValue ;
        int64_t                 int64Value ;
        uint64_t                uint64Value ;
        double                  floatValue ;
        struct CNString         stringValue ;
} ;

struct CNValuePage ;

struct CNValuePool
{
        struct CNListPool *     listPool ;
        struct CNList *         usedList ;
        struct CNList *         freeList ;
        struct CNValuePage *    firstPage ;
} ;

void
CNValuePoolInit(struct CNValuePool * dst, struct CNListPool * lpool) ;

void
CNValuePoolFree(struct CNValuePool * dst) ;

struct CNValue *
CNValueAllocate(struct CNValuePool * pool) ;

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst) ;

unsigned int
CNValueCountOfFreeItems(const struct CNValuePool * pool) ;

#endif /* CNVALUE_H */
