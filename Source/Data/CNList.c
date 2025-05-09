/*
 * @file CNList.c
 * @description Define CNList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNList.h>
#import <BasicKit/CNInterface.h>
#include <stdlib.h>

#define CNLIST_PAGE_NUM         1024

struct CNListPage {
        struct CNListPage *     next ;
        struct CNList           page[CNLIST_PAGE_NUM] ;
} ;

static struct CNListPage *
CNAllocateListPage(void)
{
        struct CNListPage * newpage = malloc(sizeof(struct CNListPage)) ;
        newpage->next = NULL ;
        return newpage ;
}

static void
CNFreeListPage(struct CNListPage * dst)
{
        free(dst) ;
}

void
CNInitListPool(struct CNListPool * dst)
{
        struct CNListPage * newpage = CNAllocateListPage() ;
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
CNDeinitListPool(struct CNListPool * dst)
{
        struct CNListPage * page = dst->firstPage ;
        while(page != NULL) {
                struct CNListPage * target = page ;
                page = target->next ;
                CNFreeListPage(target) ;
        }
}

void
CNDumpListPool(const struct CNListPool * src)
{
        unsigned int freenum = 0 ;
        const struct CNList * list ;
        for(list = src->freeList ; list != NULL ; list = list->next) {
                freenum++ ;
        }
        CNInterface()->printf("ListPool: free-num=%u\n", freenum) ;
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
                struct CNListPage * newpage = CNAllocateListPage() ;
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

struct CNList *
CNLastInList(struct CNList * list)
{
        if(list != NULL){
                struct CNList * prev = list ;
                struct CNList * next = prev->next ;
                while(next != NULL){
                        prev = next ;
                        next = prev->next ;
                }
                return prev ;
        } else {
                return NULL ;
        }
}

struct CNMemoryUsage
CNMemoryUsageOfListPool(const struct CNListPool * src)
{
        struct CNListPage * page ;

        /* allocated size */
        size_t                  asize = 0 ;
        struct CNListPage *     next ;
        for(page = src->firstPage ; page != NULL ; page = next){
                asize += sizeof(struct CNList) * CNLIST_PAGE_NUM ;
                next = page->next ;
        }
        /* free size */
        size_t                  usize = 0 ;
        struct CNList *         list ;
        for(list = src->freeList ; list != NULL ; list = list->next){
                usize += sizeof(struct CNList) ;
        }
        /* return result */
        struct CNMemoryUsage result = {
                .allocatedSize  = asize,
                .usableSize     = usize
        } ;
        return result ;
}

void
CNPrintMemoryUsage(const struct CNMemoryUsage * src)
{
        size_t usedsize ;
        if(src->allocatedSize >= src->usableSize){
                usedsize = src->allocatedSize - src->usableSize ;
        } else {
                CNInterface()->error("unexpected usableSize at %s", __func__) ;
                usedsize = src->allocatedSize ;
        }
        CNInterface()->printf("memusage: {allocated_size: %ld, usable_size: %ld, used_size: %ld}\n",
                              src->allocatedSize, src->usableSize, usedsize) ;
}

