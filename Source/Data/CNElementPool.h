/*
 * @file CNElementPool.h
 * @description Define CNElementPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNELEMENTPOOL_H
#define CNELEMENTPOOL_H

#import <BasicKit/CNPagePool.h>

struct CNElementPool
{
        struct CNPagePool       pagePool ;
        size_t                  elementSize ;
        unsigned int            elementNum ;
        struct CNList *         freeList ;
} ;

void
CNInitElementPool(struct CNElementPool * dst, size_t size, unsigned int num, struct CNListPool * lpool) ;

void
CNFreeElementPool(struct CNElementPool * dst) ;

void *
CNAllocateElement(struct CNElementPool * src) ;

void
CNFreeElement(struct CNElementPool * src, void * data) ;

#endif /* CNELEMENTPOOL_H */
