/*
 * @file CNRegisterFile.c
 * @description Define CNRegisterFile data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNRegisterFile.h>

void
CNInitRegisterFile(struct CNRegisterFile * dst, struct CNValuePool * vpool)
{
        dst->valuePool         = vpool ;

        struct CNList * list = CNAllocateList(CNListPoolInValuePool(vpool)) ;
        list->data = CNAllocateUnsignedInt(0, vpool) ;
        list->next = NULL ;
        dst->currentIndexes = list ;

        for(unsigned int i=0 ; i<CNNumberOfPagesInRegisterFile ; i++){
                dst->arrays[i] = CNAllocateArray(CNNumberOfRegistersInPage , vpool) ;
        }
}

void
CNDeinitRegisterFile(struct CNRegisterFile * dst)
{
        /* release indexes */
        struct CNList * list = dst->currentIndexes ;
        struct CNList * next ;
        for( ; list != NULL ; list = next){
                next = list->next ;
                CNReleaseValue(dst->valuePool, list->data) ;
        }
        /* release pages */
        for(unsigned int i=0 ; i<CNNumberOfPagesInRegisterFile ; i++){
                CNReleaseValue(dst->valuePool, dst->arrays[i]) ;
        }
}

