/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNValue.h"
#include <stdlib.h>

#define CNALUE_PAGE_NUM         512

struct CNValuePage {
        struct CNValuePage *    next ;
        struct CNValue          values[CNALUE_PAGE_NUM] ;
} ;

static struct CNValuePage *
CNValuePageAllocate(void)
{
        struct CNValuePage * newpage = malloc(sizeof(struct CNValuePage)) ;
        newpage->next   = NULL ;
        return newpage ;
}

static void
CNValuePageFree(struct CNValuePage * dst)
{
        free(dst) ;
}

void
CNValuePoolInit(struct CNValuePool * dst, struct CNListPool * lpool)
{
        struct CNValuePage * newpage = CNValuePageAllocate() ;
        dst->listPool           = lpool ;
        dst->firstPage          = newpage ;
        dst->usedList           = NULL ;
        dst->freeList           = NULL ;

        unsigned int i ;
        for(i = 0 ; i < CNALUE_PAGE_NUM ; i++) {
                struct CNValue * value = &(newpage->values[i]) ;
                struct CNList  * list  = CNListAllocate(lpool) ;
                list->data = value ;

                list->next    = dst->freeList ;
                dst->freeList = list ;
        }
}

void
CNValuePoolFree(struct CNValuePool * dst)
{
        /* release list */
        struct CNList * list ;
        struct CNList * next ;
        for(list = dst->usedList ; list != NULL ; list = next) {
                next = list->next ;
                CNListFree(dst->listPool, list) ;
        }
        for(list = dst->freeList ; list != NULL ; list = next) {
                next = list->next ;
                CNListFree(dst->listPool, list) ;
        }
        struct CNValuePage * page ;
        struct CNValuePage * npage ;
        for(page = dst->firstPage ; page != NULL ; page = npage) {
                npage = page->next ;
                CNValuePageFree(page) ;
        }
}

struct CNValue *
CNValueAllocate(struct CNValuePool * pool)
{
        struct CNList *  list ;
        struct CNValue * result ;
        if((list = pool->freeList) != NULL){
                result = list->data ;
                pool->freeList = list->next ;
                CNListFree(pool->listPool, list) ;
        } else {
                struct CNValuePage * newpage = CNValuePageAllocate() ;
                newpage->next   = pool->firstPage ;
                pool->firstPage = newpage ;

                unsigned int i ;
                for(i = 0 ; i < CNALUE_PAGE_NUM ; i++) {
                        struct CNValue * value = &(newpage->values[i]) ;
                        struct CNList  * vlist = CNListAllocate(pool->listPool) ;
                        vlist->data = value ;

                        vlist->next    = pool->freeList ;
                        pool->freeList = vlist ;
                }
                return CNValueAllocate(pool) ;
        }
        return result ;
}

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst)
{
        struct CNList  * vlist = CNListAllocate(pool->listPool) ;
        vlist->data    = dst ;
        vlist->next    = pool->freeList ;
        pool->freeList = vlist ;
}

unsigned int
CNValueCountOfFreeItems(const struct CNValuePool * pool)
{
        unsigned int    result = 0 ;
        struct CNList * item ;
        for(item = pool->freeList ; item != NULL ; item = item->next) {
                result++ ;
        }
        return result ;
}
