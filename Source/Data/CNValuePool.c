/*
 * @file CNValuePool.c
 * @description Define CNValuePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNString.h>
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
CNValueAllocate(CNValueType type, uint32_t size,  struct CNValuePool * pool)
{
        struct CNList *  list ;
        if((list = pool->freeList) != NULL){
                struct CNValue * result ;
                result = list->data ;
                pool->freeList = list->next ;
                CNListFree(pool->listPool, list) ;
                result->attribute = CNMakeValueAttribute(type, size) ;
                return result ;
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
                return CNValueAllocate(type, size, pool) ;
        }
}

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst)
{
        /* release context */
        switch(CNTypeOfValue(dst)){
                case CNVoidValueType:
                case CNCharValueType:
                case CNIntValueType:
                case CNFloatValueType: {
                        /* nothing havee to do */
                } break ;
                case CNStringValueType: {
                        CNStringFree(pool, &(dst->stringValue)) ;
                } break ;
        }
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

