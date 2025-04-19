/*
 * @file CNValueList.c
 * @description Define CNValueList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNValueList.h"

void
CNDeinitValueList(struct CNValueList * dst)
{
        struct CNListPool * lpool = CNListPoolInValuePool(dst->valuePool) ;
        
        struct CNList * next ;
        for(struct CNList * list = dst->firstItem ; list != NULL ; list = next) {
                next = list->next ;
                CNReleaseValue(dst->valuePool, list->data) ;
                CNFreeList(lpool, list) ;
        }
        dst->firstItem = NULL ;
}

void
CNAppendToValueList(struct CNValueList * dst, struct CNValue * src)
{
        struct CNList * newlist = CNAllocateList(CNListPoolInValuePool(dst->valuePool)) ;
        newlist->data   = src ;
        newlist->next   = NULL ;
        CNRetainValue(src) ;

        struct CNList * last = CNLastInList(dst->firstItem) ;
        if(last != NULL){
                last->next     = newlist ;
        } else {
                dst->firstItem = newlist ;
        }
}

void
CNPrependToValueList(struct CNValueList * dst, struct CNValue * src)
{
        struct CNList * newlist = CNAllocateList(CNListPoolInValuePool(dst->valuePool)) ;
        newlist->data   = src ;
        CNRetainValue(src) ;

        newlist->next  = dst->firstItem ;
        dst->firstItem = newlist ;
}

struct CNValue *
CNPopFromValueList(struct CNValueList * src)
{
        struct CNList * first = src->firstItem ;
        if(first != NULL){
                src->firstItem = first->next ;
                struct CNValue * result = first->data ;
                CNReleaseValue(src->valuePool, result) ;
                CNFreeList(CNListPoolInValuePool(src->valuePool), first) ;
                return result ;
        } else {
                return NULL ;
        }
}
