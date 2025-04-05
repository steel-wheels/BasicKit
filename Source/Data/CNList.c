/*
 * @file CNList.c
 * @description Define CNList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNList.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>
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
        newpage->next = NULL ;
        return newpage ;
}

static void
CNListPageFree(struct CNListPage * dst)
{
        free(dst) ;
}

void
CNInitListPool(struct CNListPool * dst)
{
        struct CNListPage * newpage = CNListPageAllocate() ;
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
CNFreeListPool(struct CNListPool * dst)
{
        struct CNListPage * page = dst->firstPage ;
        while(page != NULL) {
                struct CNListPage * target = page ;
                page = target->next ;
                CNListPageFree(target) ;
        }
}

void
CNDumpListPool(unsigned int indent, const struct CNListPool * src)
{
        unsigned int freenum = 0 ;
        const struct CNList * list ;
        for(list = src->freeList ; list != NULL ; list = list->next) {
                freenum++ ;
        }
        CNDumpIndent(indent) ; CNInterface()->printf("ListPool: free-num=%u\n", freenum) ;
}

struct CNList *
CNAllocateList(struct CNListPool * pool)
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

                result = CNAllocateList(pool) ;
        }
        result->next            = NULL ;
        result->attribute       = 0 ;
        result->data            = NULL ;
        return result ;
}

void
CNFreeList(struct CNListPool * pool, struct CNList * dst)
{
        dst->attribute  = 0 ;
        dst->data       = NULL ;
        dst->next       = pool->freeList ;
        pool->freeList  = dst ;
}

unsigned int
CNCountOfFreeItemsInListPool(const struct CNListPool * src)
{
        unsigned int result = 0 ;
        for(struct CNList * list = src->freeList ; list != NULL ; list = list->next){
                result += 1 ;
        }
        return result ;
}
