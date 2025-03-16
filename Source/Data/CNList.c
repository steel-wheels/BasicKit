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
        dst->used_items = NULL ;
        dst->free_items = NULL ;
        dst->first_page = newpage ;

        unsigned int i ;
        for(i=0 ; i<CNLIST_PAGE_NUM ; i++) {
                struct CNList * newitem = &(newpage->page[i]) ;
                newitem->next = dst->free_items ;
                dst->free_items = newitem ;
        }
}

void
CNListPoolFree(struct CNListPool * dst)
{
        struct CNListPage * page = dst->first_page ;
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
        if((fitem = pool->free_items) != NULL) {
                result = fitem ;
                pool->free_items = fitem->next ;
                result->next     = pool->used_items ;
                pool->used_items = result ;
        } else {
                struct CNListPage * newpage = CNListPageAllocate() ;
                newpage->next    = pool->first_page ;
                pool->first_page = newpage ;
                result = CNListAllocate(pool) ;
        }
        return result ;
}

void
CNListFree(struct CNListPool * pool, struct CNList * dst)
{
        dst->data        = NULL ;
        dst->next        = pool->free_items ;
        pool->free_items = dst ;
}
