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
        void *                  data ;
} ;

struct CNListPage ;

struct CNListPool {
        struct CNList *         free_items ;
        struct CNList *         used_items ;
        struct CNListPage *     first_page ;
} ;

void
CNListPoolInit(struct CNListPool * dst) ;

void
CNListPoolFree(struct CNListPool * dst) ;

struct CNList *
CNListAllocate(struct CNListPool * pool) ;

void
CNListFree(struct CNListPool * pool, struct CNList * dst) ;

unsigned int
CNListCountOfFreeItems(const struct CNListPool * pool) ;

#endif /* CNLIST_H */
