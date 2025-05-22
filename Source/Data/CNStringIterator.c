/*
 * @file CNStringIterator.c
 * @description Define function to iterate string list
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNStringIterator.h"
#include <stdio.h>

static inline struct CNStringValue *
popString(struct CNStringIterator * dst)
{
        struct CNValue * val = CNPopValueFromValueList(&(dst->stringList)) ;
        if(val != NULL){
                return CNCastToStringValue(val) ;
        } else {
                return NULL ;
        }
}


void
CNInitStringIterator(struct CNStringIterator * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->currentString      = NULL ;
        dst->currentIndex       = 0 ;
        CNInitValueList(&(dst->stringList), vpool) ;
}

void
CNSetStringListToStringIterator(struct CNStringIterator * dst, struct CNValueList * strlist)
{
        for(struct CNList * list = strlist->valueList ; list != NULL ; list = list->next){
                struct CNValue * val = list->data ;
                CNAppendValueToValueList(&(dst->stringList), val) ;
        }
        /* pop 1st string to iterate */
        dst->currentString      = popString(dst) ;
}

int
CNGetCharFromStringItetator(struct CNStringIterator * src)
{
        struct CNStringValue * curstr = src->currentString ;
        if(curstr == NULL){
                return EOF ;    // no more characters
        }
        if(src->currentIndex < curstr->length) {
                int result = curstr->string[src->currentIndex] ;
                src->currentIndex += 1 ;
                return result ;
        } else {
                CNReleaseValue(src->valuePool, CNSuperClassOfStringValue(curstr)) ;
                src->currentString = popString(src) ;
                src->currentIndex  = 0 ;
                return CNGetCharFromStringItetator(src) ;
        }
}

void
CNDeinitStringIterator(struct CNStringIterator * dst)
{
        struct CNStringValue * curval = dst->currentString ;
        if(curval != NULL){
                CNReleaseValue(dst->valuePool, CNSuperClassOfStringValue(curval)) ;
        }
        CNDeinitValueList(&(dst->stringList)) ;
        dst->currentString      = NULL ;
        dst->currentIndex       = 0 ;
}


