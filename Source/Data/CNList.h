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

struct CNMemoryUsage {
        size_t  allocatedSize ;
        size_t  usableSize ;
} ;

void
CNInitListPool(struct CNListPool * dst) ;

void
CNDeinitListPool(struct CNListPool * dst) ;

void
CNDumpListPool(unsigned int indent, const struct CNListPool * src) ;

struct CNList *
CNAllocateList(struct CNListPool * pool) ;

void
CNFreeList(struct CNListPool * pool, struct CNList * dst) ;

struct CNMemoryUsage
CNMemoryUsageOfListPool(const struct CNListPool * src) ;

void
CNDumpMemoryUsage(unsigned int indent, const struct CNMemoryUsage * src) ;

#endif /* CNLIST_H */
