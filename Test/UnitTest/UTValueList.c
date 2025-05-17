/*
 * @file UTValueList.c
 * @description The unit test for CNValueList
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValueList.h"
#include "UTUtils.h"

static void
appendInt(struct CNValueList * dst, uint64_t src) ;

bool
UTValueList(struct CNValuePool * vpool)
{
        bool result = true ;

        CNInterface()->printf("(%s) ValueStack test\n", __func__) ;

        CNInterface()->printf("(%s) allocation\n", __func__) ;
        struct CNValueList slist ;
        CNInitValueList(&slist, vpool) ;

        for(uint64_t i=1 ; i<100 ; i++){
                appendInt(&slist, i) ;
        }

        struct CNList * list = CNListInValueList(&slist) ;
        for(uint64_t i=1 ; list != NULL ; i++, list = list->next) {
                struct CNUnsignedIntValue * ival ;
                ival = CNCastToUnsignedIntValue(list->data) ;
                if(i != ival->value) {
                        CNInterface()->printf("(%s) unexpected value: %lu\n", __func__, ival->value) ;
                        result = false ;
                }
        }

        CNInterface()->printf("(%s) release\n", __func__) ;
        CNDeinitValueList(&slist) ;

        return checkMemoryUsage(vpool) && result ;
}

static void
appendInt(struct CNValueList * dst, uint64_t src)
{
        struct CNValuePool * vpool = dst->valuePool ;
        struct CNUnsignedIntValue * uval = CNAllocateUnsignedIntValue(vpool, src) ;
        CNAppendValueToValueList(dst, CNSuperClassOfUnsignedIntValue(uval)) ;
        CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(uval)) ;
}

