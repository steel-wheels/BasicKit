/*
 * @file CNValuePool.h
 * @description Define CNValuePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNValuePool_h
#define CNValuePool_h

#import <BasicKit/CNValue.h>
#import <BasicKit/CNScalarPool.h>
#import <BasicKit/CNArrayPool.h>

struct CNValuePool {
        struct CNScalarPool     scalarPool ;
        struct CNArrayPool      arrayPool ;
} ;

static inline void
CNInitValuePool(struct CNValuePool * dst, struct CNListPool * lpool)
{
        CNInitScalarPool(&(dst->scalarPool), CNValueSize, 1024, lpool) ;
        CNInitArrayPool(&(dst->arrayPool), lpool) ;
}

static inline void
CNDeinitValuePool(struct CNValuePool * dst)
{
        CNDeinitScalarPool(&(dst->scalarPool)) ;
        CNDeinitArrayPool(&(dst->arrayPool)) ;
}

static inline struct CNListPool *
CNListPoolInValuePool(struct CNValuePool * src)
{
        return (src->arrayPool).pagePool.listPool ;
}

static inline struct CNValue *
CNAllocateScalar(struct CNValuePool * src)
{
        return (struct CNValue *) CNAllocateScalarData(&(src->scalarPool)) ;
}

static inline void
CNFreeScalar(struct CNValuePool * src, struct CNValue * data)
{
        CNFreeScalarData(&(src->scalarPool), data) ;
}

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src) ;

struct CNMemoryUsage
CNMemoryUsageOfValuePool(const struct CNValuePool * src) ;

#endif /* CNValuePool_h */
