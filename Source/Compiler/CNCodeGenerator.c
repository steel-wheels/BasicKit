/*
 * @file CNCOddeGenerator.h
 * @description Defint byte code converter
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNCodeGenerator.h"

struct CNArrayValue *
CNGenerateCode(struct CNValueList * codelist)
{
        struct CNValuePool * vpool = codelist->valuePool ;
        unsigned int num = CNNumberOfValuesInValueList(codelist) ;

        /* increment number to append stop code */
        unsigned int totalnum = num + 1 ;

        struct CNArrayValue * result = CNAllocateArrayValue(vpool, totalnum) ;
        struct CNList * list = CNListInValueList(codelist) ;
        unsigned int    i    = 0 ;
        for( ; list != NULL ; list = list->next, i++){
                CNSetValueToArray(vpool, result, i, list->data) ;
        }

        /* append stop code */
        struct CNCodeValue * stopcode = CNAllocateStopCode(vpool) ;
        CNSetValueToArray(vpool, result, num, CNSuperClassOfCodeValue(stopcode)) ;
        CNReleaseValue(vpool, CNSuperClassOfCodeValue(stopcode)) ;

        return result ;
}
