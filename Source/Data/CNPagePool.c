/*
 * @file CNPagePool.c
 * @description Define CNPagePool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNPagePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>
#include <stdlib.h>

void
CNInitPagePool(struct CNPagePool * dst, struct CNListPool * lpool)
{
        dst->listPool   = lpool ;
        dst->freeList   = NULL ;
}

void
CNFreePagePool(struct CNPagePool * dst)
{
        struct CNList * list = dst->freeList ;
        while(list != NULL){
                struct CNList * next = list->next ;
                free(list->data) ;
                CNFreeList(dst->listPool, list) ;
                list = next ;
        }
}

void
CNDumpPagePool(unsigned int indent, const struct CNPagePool * src)
{
        struct CNList * list ;
        unsigned int freenum = 0 ;
        for(list=src->freeList ; list != NULL ; list = list->next){
                freenum += 1 ;
        }
        CNDumpIndent(indent) ;
        CNInterface()->printf("PagePool: free-num = %u\n", freenum) ;
        CNDumpListPool(indent+1, src->listPool) ;
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
                                src->freeList = list->next ;
                        } else {
                                prev->next = list->next ;
                        }
                        CNFreeList(src->listPool, list) ;
                        return data ;
                }
                prev = list ;
        }
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

unsigned int
CNCountOfFreeItemsInPagePool(const struct CNPagePool * src)
{
        unsigned int result = 0 ;
        for(struct CNList * list = src->freeList ; list != NULL ; list = list->next){
                result += 1 ;
        }
        return result ;
}
