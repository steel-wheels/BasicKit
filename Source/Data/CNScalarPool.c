/*
 * @file CNScalarPool.c
 * @description Define CNScalarPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNScalarPool.h>
#import <BasicKit/CNInterface.h>
#import "CNUtils.h"
#include <string.h>

#define MIN(A, B)       ((A) <= (B) ? (A) : (B))

static struct CNList *
allocateScalars(struct CNScalarPool * dst) ;

void
CNInitScalarPool(struct CNScalarPool * dst, size_t size, unsigned int num, struct CNListPool * lpool)
{
        CNInitPagePool(&(dst->pagePool), lpool) ;
        dst->elementSize = size ;
        dst->elementNum  = num ;

        struct CNList * list = allocateScalars(dst) ;
        dst->freeList = list ;
}

void
CNDeinitScalarPool(struct CNScalarPool * dst)
{
        struct CNList * list = dst->freeList ;
        while(list != NULL) {
                struct CNList * next = list->next ;
                CNFreeList((dst->pagePool).listPool, list) ;
                list = next ;
        }
        CNDeinitPagePool(&(dst->pagePool)) ;
}

void
CNDumpScalarPool(unsigned int indent, const struct CNScalarPool * src)
{
        unsigned int freenum = 0 ;
        for(struct CNList * list = src->freeList ; list != NULL ; list = list->next) {
                freenum += 1 ;
        }
        CNPrintIndent(indent) ; CNInterface()->printf("ScalarPool: elementSize = %lu\n", src->elementSize) ;
        CNPrintIndent(indent) ; CNInterface()->printf("ScalarPool: elementNum  = %u\n",  src->elementNum) ;
        CNPrintIndent(indent) ; CNInterface()->printf("ScalarPool: freeNum     = %u\n",  freenum) ;
        CNDumpPagePool(indent+1, &(src->pagePool)) ;
}

void *
CNAllocateScalarData(struct CNScalarPool * src)
{
        /* search free elements */
        struct CNList * list = src->freeList ;
        if(list != NULL){
                void * data = list->data ;
                src->freeList = list->next ;
                CNFreeList((src->pagePool).listPool, list) ;
                return data ;
        } else {
                struct CNList * list = allocateScalars(src) ;
                src->freeList = list ;
                return CNAllocateScalarData(src) ;
        }
}

static struct CNList *
allocateScalars(struct CNScalarPool * dst)
{
        unsigned int elmnum  = dst->elementNum ;
        size_t       elmsize = dst->elementSize ;
        uint8_t *    page    = CNAllocatePage(&(dst->pagePool), elmsize * elmnum) ;
        struct CNList * result = NULL ;
        size_t fillsize = MIN(sizeof(uint64_t), elmsize) ;
        for(unsigned int i=0 ;  i<elmnum ; i++, page+=elmsize) {
                /* fill 1st 8 bytes */
                memset(page, 0, fillsize) ;
                /* make list */
                struct CNList * list = CNAllocateList((dst->pagePool).listPool) ;
                list->attribute = elmsize ;
                list->next      = result ;
                list->data      = page ;
                result          = list ;
        }
        return result ;
}

void
CNFreeScalarData(struct CNScalarPool * src, void * data)
{
        struct CNList * list = CNAllocateList((src->pagePool).listPool) ;
        list->attribute = src->elementSize ;
        list->next      = src->freeList ;
        list->data      = data ;
        src->freeList   = list ;
}

struct CNMemoryUsage
CNMemoryUsageOfScalarPool(const struct CNScalarPool * src)
{
        struct CNMemoryUsage pusage = CNMemoryUsageOfPagePool(&(src->pagePool)) ;

        /* size of usable scalars */
        size_t          usize = 0 ;
        struct CNList * list ;
        for(list = src->freeList ; list != NULL ; list = list->next){
                usize += src->elementSize ;
        }

        struct CNMemoryUsage result = {
                .allocatedSize  = pusage.allocatedSize,
                .usableSize     = pusage.usableSize + usize
        } ;
        return result ;
}
