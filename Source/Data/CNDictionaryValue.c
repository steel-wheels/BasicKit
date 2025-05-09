/*
 * @file CNDictionaryValue.c
 * @description Define dictionary value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */


#include "CNDictionaryValue.h"
#include "CNNullValue.h"
#include "CNStringValue.h"
#include "CNValuePool.h"
#include "CNInterface.h"

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printValues(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForDictionaryValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printValues ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNDictionaryValue *
CNAllocateDictionaryValue(struct CNValuePool * vpool)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForDictionaryValue() ;
        struct CNDictionaryValue * newval ;
        newval = (struct CNDictionaryValue *) CNAllocateValue(vpool, CNDictionaryType, vfunc) ;
        newval->elementNum      = 0 ;
        newval->elementList     = NULL ;
        return newval ;
}

#define ELEMENT_NUM_IN_PAGE     (32 * 2)

static struct CNValue **
allocateElements(struct CNValuePool * vpool)
{
        struct CNValue ** result ;
        unsigned int pagenum = ELEMENT_NUM_IN_PAGE ;
        result = (struct CNValue **) CNAllocateArrayData(&(vpool->arrayPool), pagenum) ;

        struct CNValue ** ptr    = result ;
        struct CNValue ** endptr = ptr + ELEMENT_NUM_IN_PAGE ;
        for( ; ptr < endptr ; ptr++){
                *ptr = CNSuperClassOfNullValue(CNAllocateNullValue()) ;
        }

        return result ;
}

static void
releaseElements(struct CNValuePool * vpool, struct CNValue ** elm)
{
        struct CNValue ** ptr    = elm ;
        struct CNValue ** endptr = ptr + ELEMENT_NUM_IN_PAGE ;
        for( ; ptr < endptr ; ptr++){
                CNReleaseValue(vpool, CNSuperClassOfNullValue(CNAllocateNullValue())) ;
        }
        CNFreeArrayData(&(vpool->arrayPool), ELEMENT_NUM_IN_PAGE, elm) ;
}

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNDictionaryValue * dict = CNCastToDictionaryValue(val) ;
        struct CNList *list, *next ;
        for(list = dict->elementList ; list != NULL ; list = next){
                next = list->next ;
                releaseElements(vpool, list->data) ;
        }
}

static void printElement(struct CNValue ** elm)
{
        struct CNValue *  key   = elm[0] ;
        struct CNValue *  value = elm[1] ;
        if(CNCastToStringValue(key) != NULL){
                CNPrintValue(key) ;
                CNInterface()->printf(":") ;
                CNPrintValue(value) ;
        }
}

static void printValues(struct CNValue * val)
{
        struct CNDictionaryValue * dict = CNCastToDictionaryValue(val) ;
        struct CNList *list ;
        CNInterface()->printf("[") ;
        for(list = dict->elementList ; list != NULL ; list = list->next){
                struct CNValue ** elements = list->data ;
                struct CNValue ** endptr   = elements + ELEMENT_NUM_IN_PAGE ;
                bool   is1st = true ;
                for( ; elements < endptr ; elements+=2){
                        if(!is1st){
                                CNInterface()->printf(", ") ;
                                is1st = false ;
                        }
                        printElement(elements) ;
                }
        }
        CNInterface()->printf("]") ;
}

