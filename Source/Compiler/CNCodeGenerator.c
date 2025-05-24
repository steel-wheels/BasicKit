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

        struct CNArrayValue * result = CNAllocateArrayValue(vpool, num) ;
        struct CNList * list = CNListInValueList(codelist) ;
        unsigned int    i    = 0 ;
        for( ; list != NULL ; list = list->next, i++){
                CNSetValueToArray(vpool, result, i, list->data) ;
        }
        return result ;
}
