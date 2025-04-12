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
        struct CNArrayPool      _arrayPool ;
} ;

static inline void
CNInitValuePool(struct CNValuePool * dst, struct CNListPool * lpool)
{
        CNInitScalarPool(&(dst->scalarPool), sizeof(struct CNValue), 1024, lpool) ;
        CNInitArrayPool(&(dst->_arrayPool), sizeof(struct CNValue *), lpool) ;
}

static inline void
CNDeinitValuePool(struct CNValuePool * dst)
{
        CNDeinitScalarPool(&(dst->scalarPool)) ;
        CNDeinitArrayPool(&(dst->_arrayPool)) ;
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

static inline struct CNValue **
CNAllocateArrayElements(struct CNValuePool * src, unsigned int elmnum)
{
        return (struct CNValue **) CNAllocateArrayData(&(src->_arrayPool), elmnum) ;
}

static inline void
CNFreeArrayElements(struct CNValuePool * src, unsigned int elmnum, struct CNValue ** data)
{
        CNFreeArrayData(&(src->_arrayPool), elmnum, data) ;
}

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src) ;

static inline unsigned int
CNCountOfFreeScalarItemsInValuePool(const struct CNValuePool * src)
{
        return CNCountOfFreeItemsInScalarPool(&(src->scalarPool)) ;
}

static inline unsigned int
CNCountOfFreeArrayItemsInValuePool(const struct CNValuePool * src)
{
        return CNCountOfFreeItemsInArrayPool(&(src->_arrayPool)) ;
}

#endif /* CNValuePool_h */
