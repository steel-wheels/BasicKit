/*
 * @file UTStringIterator.c
 * @description The unit test for CNStringIterator
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTStringIterator.h"
#include "UTUtils.h"
#include <string.h>
#include <stdio.h>

bool
UTStringIterator(struct CNValuePool * vpool)
{
        bool    result = true ;

#       define STRING_NUM       5
        char            c ;
        unsigned int    index = 0 ;

        CNInterface()->printf("(%s) make source string\n", __func__) ;
        char source[STRING_NUM][STRING_NUM +1] ;
        for(unsigned int n=0 ; n<STRING_NUM ; n++){
                unsigned int w ;
                for(w=0 ; w<n ; w++){
                        source[n][w] = 'a' + (index % 26) ;
                        index++ ;
                }
                source[n][w] = '\0' ;
        }
        unsigned int totalnum = index ;

        CNInterface()->printf("(%s) make value list\n", __func__) ;
        struct CNValueList vlist ;
        CNInitValueList(&vlist, vpool) ;
        for(unsigned int n=0 ; n<STRING_NUM ; n++){
                const char * s = source[n] ;
                struct CNStringValue * str = CNAllocateStringValue(vpool, (unsigned int) strlen(s), s) ;
                CNAppendValueToValueList(&vlist, CNSuperClassOfStringValue(str)) ;
                CNReleaseValue(vpool, CNSuperClassOfStringValue(str)) ;
        }

        CNInterface()->printf("(%s) test string iterator\n", __func__) ;
        struct CNStringIterator iter ;
        CNInitStringIterator(&iter, &vlist, vpool) ;
        index = 0 ;
        for(unsigned int i=0 ; i<totalnum ; i++){
                int exp = 'a' + (index % 26) ; index++ ;
                int ch  = CNGetCharFromStringItetator(&iter) ;
                if(ch != exp){
                        CNInterface()->error("[Error] Unmatch %c <-> %c at %u\n",
                                             ch, exp,index) ;
                        result = false ;
                        break ;
                }
        }
        if(CNGetCharFromStringItetator(&iter) != EOF){
                CNInterface()->error("[Error] End of string is expected\n") ;
                result = false ;
        }

        CNDeinitValueList(&vlist) ;
        CNDeinitStringIterator(&iter) ;
        
        return checkMemoryUsage(vpool) && result ;
}
