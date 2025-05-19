/*
 * @file CNValueList.c
 * @description Define value list data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNValueList.h"
#include "CNValuePool.h"
#include "CNInterface.h"

void
CNDeinitValueList(struct CNValueList * dst)
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
CNAppendValueToValueList(struct CNValueList * dst, struct CNValue * src)
{
        CNRetainValue(src) ;

        struct CNList * newlist = CNAllocateList(CNListPoolInValuePool(dst->valuePool)) ;
        newlist->data = src ;
        newlist->next = NULL ;

        struct CNList * last = CNLastInList(dst->valueList) ;
        if(last != NULL){
                last->next = newlist ;
        } else {
                dst->valueList = newlist ;
        }
}

struct CNValue *
CNPopValueFromValueList(struct CNValueList * dst)
{
        struct CNList * list ;
        if((list = dst->valueList) != NULL){
                struct CNList *  next   = list->next ;
                struct CNValue * result = list->data ;
                dst->valueList          = next ;
                CNFreeList(CNListPoolInValuePool(dst->valuePool),  list) ;
                return result ;
        } else {
                return NULL ;
        }
}

void
CNPrintValueList(struct CNValueList * src)
{
        bool is1st = true ;

        CNInterface()->printf("{") ;
        for(struct CNList * list = src->valueList ; list != NULL ; list = list->next){
                if(!is1st){
                        CNInterface()->printf(", ") ; is1st = false ;
                }
                CNPrintValue(list->data) ;
        }
        CNInterface()->printf("}") ;
}
