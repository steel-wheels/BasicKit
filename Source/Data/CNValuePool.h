/*
 * @file CNValuePool.h
 * @description Define CNValuePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUEPOOL_H
#define CNVALUEPOOL_H

#import <BasicKit/CNValue.h>

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
CNValueAllocate(CNValueType type, uint32_t size,  struct CNValuePool * pool) ;

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst) ;

unsigned int
CNValueCountOfFreeItems(const struct CNValuePool * pool) ;

#endif /* CNVALUEPOOL_H */
