/*
 * @file CNProgram.c
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNProgram.h>

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool)
{
        dst->valuePool = vpool ;

        struct CNList * list = CNAllocateList(CNListPoolInValuePool(vpool)) ;
        list->next = NULL ;
        list->data = CNAllocateDictionary(vpool) ;
        dst->variableTables = list ;

        CNInitValueList(&(dst->program), vpool) ;
}

void
CNDeinitProgram(struct CNProgram * dst)
{
        struct CNValuePool * vpool = dst->valuePool ;
        struct CNListPool  * lpool = CNListPoolInValuePool(vpool) ;
        struct CNList *list, *next ;
        for(list = dst->variableTables ; list != NULL ; list = next){
                next = list->next ;
                CNReleaseValue(vpool, list->data) ;
                CNFreeList(lpool, list) ;
        }
        dst->variableTables = NULL ;
        CNDeinitValueList(&(dst->program)) ;
}


