/*
 * @file CNScalarPool.h
 * @description Define CNScalarPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNScalarPool_H
#define CNScalarPool_H

#import <BasicKit/CNPagePool.h>

struct CNScalarPool
{
        struct CNPagePool       pagePool ;
        size_t                  elementSize ;
        unsigned int            elementNum ;
        struct CNList *         freeList ;
} ;

void
CNInitScalarPool(struct CNScalarPool * dst, size_t size, unsigned int num, struct CNListPool * lpool) ;

void
CNDeinitScalarPool(struct CNScalarPool * dst) ;

void
CNDumpScalarPool(unsigned int indent, const struct CNScalarPool * src) ;

void *
CNAllocateScalar(struct CNScalarPool * src) ;

void
CNFreeScalar(struct CNScalarPool * src, void * data) ;

unsigned int
CNCountOfFreeItemsInScalarPool(const struct CNScalarPool * src) ;

#endif /* CNScalarPool_H */
