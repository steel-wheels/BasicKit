/*
 * @file CNList.h
 * @description Define CNList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNLIST_H
#define CNLIST_H

#import <BasicKit/CNType.h>

struct CNList {
        struct CNList *         next ;
        uint64_t                attribute ;
        void *                  data ;
} ;

struct CNListPage ;

struct CNListPool {
        struct CNList *         freeList ;
        struct CNListPage *     firstPage ;
} ;

void
CNInitListPool(struct CNListPool * dst) ;

void
CNFreeListPool(struct CNListPool * dst) ;

void
CNDumpListPool(unsigned int indent, const struct CNListPool * src) ;

struct CNList *
CNAllocateList(struct CNListPool * pool) ;

void
CNFreeList(struct CNListPool * pool, struct CNList * dst) ;

unsigned int
CNCountOfFreeItemsInListPool(const struct CNListPool * src) ;

#endif /* CNLIST_H */
