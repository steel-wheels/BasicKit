/*
 * @file CNList.c
 * @description Define CNList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNList.h"
#include <stdlib.h>

#define CNLIST_PAGE_NUM         1024

struct CNListPage {
        struct CNListPage *     next ;
        struct CNList           page[CNLIST_PAGE_NUM] ;
} ;

static struct CNListPage *
CNListPageAllocate(void)
{
        struct CNListPage * newpage = malloc(sizeof(struct CNListPage)) ;
        newpage->next   = NULL ;
        return newpage ;
}

static void
CNListPageFree(struct CNListPage * dst)
{
        free(dst) ;
}

void
CNListPoolInit(struct CNListPool * dst)
{
        struct CNListPage * newpage = CNListPageAllocate() ;
        dst->usedList  = NULL ;
        dst->freeList  = NULL ;
        dst->firstPage = newpage ;

        unsigned int i ;
        for(i=0 ; i<CNLIST_PAGE_NUM ; i++) {
                struct CNList * newitem = &(newpage->page[i]) ;
                newitem->next = dst->freeList ;
                dst->freeList = newitem ;
        }
}

void
CNListPoolFree(struct CNListPool * dst)
{
        struct CNListPage * page = dst->firstPage ;
        while(page != NULL) {
                struct CNListPage * target = page ;
                page = target->next ;
                CNListPageFree(target) ;
        }
}

struct CNList *
CNListAllocate(struct CNListPool * pool)
{
        struct CNList * result ;
        struct CNList * fitem ;
        if((fitem = pool->freeList) != NULL) {
                result = fitem ;
                pool->freeList = fitem->next ;
        } else {
                struct CNListPage * newpage = CNListPageAllocate() ;
                newpage->next   = pool->firstPage ;
                pool->firstPage = newpage ;

                unsigned int i ;
                for(i=0 ; i<CNLIST_PAGE_NUM ; i++){
                        struct CNList * newlist = &(newpage->page[i]) ;
                        newlist->next  = pool->freeList ;
                        pool->freeList = newlist ;
                }

                result = CNListAllocate(pool) ;
        }
        result->next = NULL ;
        result->data = NULL ;
        return result ;
}

void
CNListFree(struct CNListPool * pool, struct CNList * dst)
{
        dst->data      = NULL ;
        dst->next      = pool->freeList ;
        pool->freeList = dst ;
}

unsigned int
CNListCountOfFreeItems(const struct CNListPool * pool)
{
        unsigned int    result = 0 ;
        struct CNList * item ;
        for(item = pool->freeList ; item != NULL ; item = item->next) {
                result++ ;
        }
        return result ;
}
