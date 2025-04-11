/*
 * @file CNArrayPool.h
 * @description Define CNArrayPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNARRAYPOOL_H
#define CNARRAYPOOL_H

#import <BasicKit/CNPagePool.h>

struct CNArrayPool
{
        struct CNPagePool       pagePool ;
        size_t                  elementSize ;
} ;

static inline void
CNInitArrayPool(struct CNArrayPool * dst, size_t size, struct CNListPool * lpool)
{
        CNInitPagePool(&(dst->pagePool), lpool) ;
        dst->elementSize = size ;
}

static inline void
CNDeinitArrayPool(struct CNArrayPool * dst)
{
        CNDeinitPagePool(&(dst->pagePool)) ;
}

static inline size_t
CNArrayPageSize(size_t elmsize, unsigned int elmnum)
{
        static const size_t UNIT_SIZE = 512 ;
        size_t unum = (elmsize * elmnum + UNIT_SIZE - 1) / UNIT_SIZE ;
        return unum * UNIT_SIZE ;

}

static inline void *
CNAllocateArrayData(struct CNArrayPool * src, unsigned int elmnum)
{
        size_t size = CNArrayPageSize(src->elementSize, elmnum) ;
        return CNAllocatePage(&(src->pagePool), size) ;
}

static inline void
CNFreeArrayData(struct CNArrayPool * src, unsigned int elmnum, void * data)
{
        size_t size = CNArrayPageSize(src->elementSize, elmnum) ;
        CNFreePage(&(src->pagePool), size, data) ;
}

void
CNDumpArrayPool(unsigned int indent, const struct CNArrayPool * src) ;

static inline unsigned int
CNCountOfFreeItemsInArrayPool(const struct CNArrayPool * src)
{
        return CNCountOfFreeItemsInPagePool(&(src->pagePool)) ;
}

#endif /* CNARRAYPOOL_H */
