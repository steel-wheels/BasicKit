/*
 * @file CNValueStack.c
 * @description Define value stack data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNValueStack.h"
#include "CNValuePool.h"
#include "CNList.h"

void
CNDeinitValueStack(struct CNValueStack * dst)
{
        struct CNValuePool * vpool = dst->valuePool ;
        struct CNList *list, *next ;
        for(list = dst->valueList ; list != NULL ; list = next){
                next = list->next ;
                CNReleaseValue(vpool, list->data) ;
                CNFreeList(CNListPoolInValuePool(vpool), list) ;
        }
}

void
CNPushValueToStack(struct CNValueStack * dst, struct CNValue * src)
{
        CNRetainValue(src) ;
        struct CNList * newlist = CNAllocateList(CNListPoolInValuePool(dst->valuePool)) ;
        newlist->data  = src ;
        newlist->next  = dst->valueList ;
        dst->valueList = newlist ;
}

/* the return value will be NULL */
struct CNValue *
CNPopValueFromStack(struct CNValueStack * src)
{
        struct CNValue * result ;
        struct CNList  * list ;
        if((list = src->valueList) != NULL){
                result = list->data ;
                src->valueList = list->next ;
                CNFreeList(CNListPoolInValuePool(src->valuePool), list) ;
        } else {
                result = NULL ; // no data in the stack
        }
        return result ;
}

