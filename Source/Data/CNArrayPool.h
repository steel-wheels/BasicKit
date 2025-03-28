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
CNFreeArrayPool(struct CNArrayPool * dst)
{
        CNFreePagePool(&(dst->pagePool)) ;
}

static inline void *
CNAllocateArray(struct CNArrayPool * src, unsigned int elmnum)
{
        size_t reqsize  = src->elementSize * elmnum ;
        return CNAllocatePage(&(src->pagePool), reqsize) ;
}

static inline void
CNFreeArray(struct CNArrayPool * src, unsigned int elmnum, void * data)
{
        size_t datasize = src->elementSize * elmnum ;
        CNFreePage(&(src->pagePool), datasize, data) ;
}

#endif /* CNARRAYPOOL_H */
