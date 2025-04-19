/*
 * @file CNValueList.c
 * @description Define CNValueList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValueList.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

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
                CNFreeList(CNListPoolInValuePool(src->valuePool), first) ;
                return result ;
        } else {
                return NULL ;
        }
}

void
CNDumpValueList(unsigned int indent, const struct CNValueList * src)
{
        struct CNList * list = src->firstItem ;
        CNDumpIndent(indent) ; CNInterface()->printf("{\n") ;
        for( ; list != NULL ; list = list->next) {
                CNDumpIndent(indent+1) ; CNDumpValue(0, list->data) ;
        }
        CNDumpIndent(indent) ; CNInterface()->printf("}\n") ;
}
