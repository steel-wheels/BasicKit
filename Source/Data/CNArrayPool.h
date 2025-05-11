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
} ;

static inline void
CNInitArrayPool(struct CNArrayPool * dst, struct CNListPool * lpool)
{
        CNInitPagePool(&(dst->pagePool), lpool) ;
}

static inline void
CNDeinitArrayPool(struct CNArrayPool * dst)
{
        CNDeinitPagePool(&(dst->pagePool)) ;
}

static inline void *
CNAllocateArrayData(struct CNArrayPool * src, size_t size)
{
        return CNAllocatePage(&(src->pagePool), size) ;
}

static inline void
CNFreeArrayData(struct CNArrayPool * src, size_t size, void * data)
{
        CNFreePage(&(src->pagePool), size, data) ;
}

void
CNDumpArrayPool(unsigned int indent, const struct CNArrayPool * src) ;

static inline struct CNMemoryUsage
CNMemoryUsageOfArrayPool(const struct CNArrayPool * src)
{
        return CNMemoryUsageOfPagePool(&(src->pagePool)) ;
}

#endif /* CNARRAYPOOL_H */
