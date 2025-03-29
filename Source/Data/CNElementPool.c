/*
 * @file CNElementPool.c
 * @description Define CNElementPool data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */


#import <BasicKit/CNElementPool.h>

static struct CNList *
allocateElements(struct CNElementPool * dst) ;

void
CNInitElementPool(struct CNElementPool * dst, size_t size, unsigned int num, struct CNListPool * lpool)
{
        CNInitPagePool(&(dst->pagePool), lpool) ;
        dst->elementSize = size ;
        dst->elementNum  = num ;

        struct CNList * list = allocateElements(dst) ;
        dst->freeList = list ;
}

void
CNFreeElementPool(struct CNElementPool * dst)
{
        struct CNList * list = dst->freeList ;
        while(list != NULL) {
                struct CNList * next = list->next ;
                CNFreeList((dst->pagePool).listPool, list) ;
                list = next ;
        }
        CNFreePagePool(&(dst->pagePool)) ;
}

void *
CNAllocateElement(struct CNElementPool * src)
{
        /* search free elements */
        struct CNList * list = src->freeList ;
        if(list != NULL){
                void * data = list->data ;
                src->freeList = list->next ;
                CNFreeList((src->pagePool).listPool, list) ;
                return data ;
        } else {
                struct CNList * list = allocateElements(src) ;
                src->freeList = list ;
                return CNAllocateElement(src) ;
        }
}

static struct CNList *
allocateElements(struct CNElementPool * dst)
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
CNFreeElement(struct CNElementPool * src, void * data)
{
        struct CNList * list = CNAllocateList((src->pagePool).listPool) ;
        list->attribute = src->elementSize ;
        list->next      = src->freeList ;
        list->data      = data ;
        src->freeList   = list ;
}

