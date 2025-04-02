/*
 * @file CNScalarPool.c
 * @description Define CNScalarPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNScalarPool.h>
#import <BasicKit/CNUtils.h>
#include <stdio.h>

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
CNFreeScalarPool(struct CNScalarPool * dst)
{
        struct CNList * list = dst->freeList ;
        while(list != NULL) {
                struct CNList * next = list->next ;
                CNFreeList((dst->pagePool).listPool, list) ;
                list = next ;
        }
        CNFreePagePool(&(dst->pagePool)) ;
}

void
CNDumpScalarPool(unsigned int indent, const struct CNScalarPool * src)
{
        unsigned int freenum = 0 ;
        for(struct CNList * list = src->freeList ; list != NULL ; list = list->next) {
                freenum += 1 ;
        }
        CNDumpIndent(indent) ; printf("ScalarPool: elementSize = %lu\n", src->elementSize) ;
        CNDumpIndent(indent) ; printf("ScalarPool: elementNum  = %u\n",  src->elementNum) ;
        CNDumpIndent(indent) ; printf("ScalarPool: freeNum     = %u\n",  freenum) ;
        CNDumpPagePool(indent+1, &(src->pagePool)) ;
}

void *
CNAllocateScalar(struct CNScalarPool * src)
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
                return CNAllocateScalar(src) ;
        }
}

static struct CNList *
allocateScalars(struct CNScalarPool * dst)
{
        unsigned int elmnum  = dst->elementNum ;
        size_t       elmsize = dst->elementSize ;
        uint8_t *    page    = CNAllocatePage(&(dst->pagePool), elmsize * elmnum) ;
        struct CNList * result = NULL ;
        for(unsigned int i=0 ;  i<elmnum ; i++, page+=elmsize) {
                struct CNList * list = CNAllocateList((dst->pagePool).listPool) ;
                list->attribute = elmsize ;
                list->next      = result ;
                list->data      = page ;
                result          = list ;
        }
        return result ;
}

void
CNFreeScalar(struct CNScalarPool * src, void * data)
{
        struct CNList * list = CNAllocateList((src->pagePool).listPool) ;
        list->attribute = src->elementSize ;
        list->next      = src->freeList ;
        list->data      = data ;
        src->freeList   = list ;
}

unsigned int
CNCountOfFreeItemsInScalarPool(const struct CNScalarPool * src)
{
        unsigned int result = 0 ;
        for(struct CNList * list = src->freeList ; list != NULL ; list = list->next){
                result += 1 ;
        }
        return result ;
}
