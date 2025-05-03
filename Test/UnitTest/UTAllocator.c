/*
 * @file UTAllocator.c
 * @description The unit test to allocate value
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNHeader.h>
#include <stdio.h>

static bool
UTStringAllocator(void) ;

bool UTAllocator(void)
{
        bool res0 = UTStringAllocator() ;
        return res0 ;
}

static inline char
int2char(unsigned int idx)
{
        return 'a' + (idx % 26) ;
}

static bool
UTStringAllocator(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) allocate state\n", __func__) ;
#       define STR_SIZE 777
#       define LINE_NUM 3
        struct CNValueList strlist ;
        CNInitStringList(&strlist, &vpool) ;
        for(unsigned int i=0 ; i<LINE_NUM ; i++){
                for(unsigned int j=0; j<STR_SIZE ; j++){
                        CNPutCharIntoStringList(&strlist, int2char(j)) ;
                }
                CNPutCharIntoStringList(&strlist, '\n') ;
        }

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;
        CNPrintValueList(&strlist) ;
        printf("\n") ;
        
        printf("(%s) compare state\n", __func__) ;
        struct CNStringListIterator iter ;
        CNInitStringListIterator(&iter, &vpool, &strlist) ;
        for(unsigned int i=0 ; i<LINE_NUM ; i++){
                for(unsigned int j=0; j<STR_SIZE ; j++){
                        char c = CNGetCharacterFromStringListIterator(&iter) ;
                        if(int2char(j) != c){
                                printf("(%s) [Error] failed to compare %c <-> %c at %u\n", __func__,
                                       int2char(j), c, j) ;
                                result = false ;
                                break ;
                        }
                }
        }
        if(CNGetCharacterFromStringListIterator(&iter) != EOF){
                printf("(%s) EOF is not found\n", __func__) ;
                result = false ;
        }
        CNDeinitStringListIterator(&iter) ;
        CNDeinitValueList(&strlist) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) check state\n", __func__) ;
        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        printf("(%s) Final state\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        return result ;
}

