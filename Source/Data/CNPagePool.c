/*
 * @file CNPagePool.c
 * @description Define CNPagePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNPagePool.h>
#import <BasicKit/CNInterface.h>
#import "CNUtils.h"
#include <stdlib.h>

void
CNInitPagePool(struct CNPagePool * dst, struct CNListPool * lpool)
{
        dst->listPool           = lpool ;
        dst->freeList           = NULL ;
        dst->allocatedSize      = 0 ;
}

void
CNDeinitPagePool(struct CNPagePool * dst)
{
        struct CNList * list = dst->freeList ;
        while(list != NULL){
                struct CNList * next = list->next ;
                free(list->data) ;
                CNFreeList(dst->listPool, list) ;
                list = next ;
        }
}

void *
CNAllocatePage(struct CNPagePool * src, size_t reqsize)
{
        struct CNList * list ;
        struct CNList * prev = NULL ;
        for(list = src->freeList ; list != NULL ; list = list->next) {
                if(list->attribute == reqsize) {
                        void * data = list->data ;
                        if(prev != NULL){
                                prev->next = list->next ;
                        } else {
                                src->freeList = list->next ;
                        }
                        CNFreeList(src->listPool, list) ;
                        return data ;
                }
                prev = list ;
        }
        src->allocatedSize += reqsize ;
        return malloc(reqsize) ;
}

void
CNFreePage(struct CNPagePool * src, size_t size, void * data)
{
        struct CNList * list = CNAllocateList(src->listPool) ;
        list->attribute = size ;
        list->data      = data ;
        list->next      = src->freeList ;
        src->freeList   = list ;
}

void
CNDumpPagePool(unsigned int indent, const struct CNPagePool * src)
{
        struct CNList * list ;
        size_t fsize = 0 ;
        for(list=src->freeList ; list != NULL ; list = list->next){
                fsize += (size_t) list->attribute ;
        }
        CNPrintIndent(indent) ; CNInterface()->printf("PagePool: alloc-size = %d\n", src->allocatedSize) ;
        CNPrintIndent(indent) ; CNInterface()->printf("PagePool: free-size  = %d\n", fsize) ;
        CNDumpListPool(indent+1, src->listPool) ;
}

struct CNMemoryUsage
CNMemoryUsageOfPagePool(const struct CNPagePool * src)
{
        /* free size */
        size_t          fsize = 0 ;
        struct CNList * list ;
        for(list = src->freeList ; list != NULL ; list = list->next){
                fsize += (size_t) list->attribute ;
        }
        struct CNMemoryUsage result = {
                .allocatedSize  = src->allocatedSize,
                .usableSize     = fsize
        } ;
        return result ;
}
