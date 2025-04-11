/*
 * @file CNPagePool.h
 * @description Define CNPagePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNPAGEPOOL_H
#define CNPAGEPOOL_H

#import <BasicKit/CNType.h>
#import <BasicKit/CNList.h>

struct CNPagePool
{
        struct CNListPool *     listPool ;
        /*
         * attribute:   page size
         * data:        page data
         */
        struct CNList *         freeList ;
} ;

void
CNInitPagePool(struct CNPagePool * dst, struct CNListPool * lpool) ;

void
CNDeinitPagePool(struct CNPagePool * dst) ;

void
CNDumpPagePool(unsigned int indent, const struct CNPagePool * src) ;

void *
CNAllocatePage(struct CNPagePool * src, size_t reqsize) ;

void
CNFreePage(struct CNPagePool * src, size_t size, void * data) ;

unsigned int
CNCountOfFreeItemsInPagePool(const struct CNPagePool * src) ;

#endif /* CNPAGEPOOL_H */
