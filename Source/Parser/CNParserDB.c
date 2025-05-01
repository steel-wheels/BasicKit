/*
 * @file CNParserDB.c
 * @description Define CNParserDB data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNParserDB.h"

void
CNInitParserDB(struct CNParserDB * pdb, struct CNValuePool * vpool)
{
        pdb->valuePool = vpool ;

        struct CNList * list = CNAllocateList(CNListPoolInValuePool(vpool)) ;
        list->next = NULL ;
        list->data = CNAllocateDictionary(vpool) ;
        pdb->variableTables = list ;

        CNInitValueList(&(pdb->program), vpool) ;
}

void
CNDeinitParserDB(struct CNParserDB * pdb)
{
        struct CNValuePool * vpool = pdb->valuePool ;
        struct CNListPool  * lpool = CNListPoolInValuePool(vpool) ;
        struct CNList *list, *next ;
        for(list = pdb->variableTables ; list != NULL ; list = next){
                next = list->next ;
                CNReleaseValue(vpool, list->data) ;
                CNFreeList(lpool, list) ;
        }
        pdb->variableTables = NULL ;
        CNDeinitValueList(&(pdb->program)) ;
}


