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

#define ELEMENT_NUM_IN_PAGE             32

static struct CNValue ** allocateElements(struct CNValuePool * vpool) ;
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
        newval->elementList     = NULL ;
        return newval ;
}

void
CNSetValueToDictionary(struct CNValuePool * vpool, struct CNDictionaryValue * dst, struct CNStringValue * key, struct CNValue * newvalue)
{
        /* replace current value */
        struct CNList * list ;
        for(list = dst->elementList ; list != NULL ; list = list->next){
                struct CNValue ** ptr    = list->data ;
                struct CNValue ** endptr = ptr + ELEMENT_NUM_IN_PAGE ;
                for( ; ptr < endptr ; ptr += 2){
                        struct CNStringValue * keyval = CNCastToStringValue(ptr[0]) ;
                        if(keyval != NULL){
                                if(CNCompareStringValue(keyval, key) == 0){
                                        /* the key is alread set */
                                        CNRetainValue(newvalue) ;
                                        CNReleaseValue(vpool, ptr[1]) ;
                                        ptr[1] = newvalue ;
                                        return ; // done setting
                                }
                        }
                }
        }
        /* use empty slot */
        for(list = dst->elementList ; list != NULL ; list = list->next){
                struct CNValue ** ptr    = list->data ;
                struct CNValue ** endptr = ptr + ELEMENT_NUM_IN_PAGE ;
                for( ; ptr < endptr ; ptr += 2){
                        struct CNStringValue * keyval = CNCastToStringValue(ptr[0]) ;
                        if(keyval == NULL){
                                CNRetainValue(CNSuperClassOfStringValue(key)) ;
                                CNReleaseValue(vpool, ptr[0]) ;
                                ptr[0] = CNSuperClassOfStringValue(key) ;

                                CNRetainValue(newvalue) ;
                                CNReleaseValue(vpool, ptr[1]) ;
                                ptr[1] = newvalue ;

                                return ;
                        }
                }
        }
        /* no key and no empty slot */
        struct CNList *   newlist = CNAllocateList(CNListPoolInValuePool(vpool)) ;
        struct CNValue ** newpage = allocateElements(vpool) ;
        newlist->data = newpage ;
        newlist->next = NULL ;
        if(dst->elementList != NULL){
                struct CNList * last = CNLastInList(dst->elementList) ;
                last->next = newlist ;
        } else {
                dst->elementList = newlist ;
        }
        /* set new slot */
        newpage[0] = CNSuperClassOfStringValue(key) ;
        newpage[1] = newvalue ;
        CNRetainValue(newpage[0]) ;
        CNRetainValue(newpage[1]) ;
}

struct CNValue *
CNValueForKeyInDictionary(struct CNDictionaryValue * dst, struct CNStringValue * key)
{
        struct CNList * list ;
        for(list = dst->elementList ; list != NULL ; list = list->next){
                struct CNValue ** ptr    = list->data ;
                struct CNValue ** endptr = ptr + ELEMENT_NUM_IN_PAGE ;
                for( ; ptr < endptr ; ptr += 2){
                        struct CNStringValue * keyval = CNCastToStringValue(ptr[0]) ;
                        if(keyval != NULL){
                                if(CNCompareStringValue(keyval, key) == 0){
                                        return ptr[1] ; // value for key
                                }
                        }
                }
        }
        return NULL ;
}

static struct CNValue **
allocateElements(struct CNValuePool * vpool)
{
        struct CNValue ** result ;
        result = (struct CNValue **) CNAllocateArrayData(&(vpool->arrayPool),
                                                         sizeof(struct CNValue *) * ELEMENT_NUM_IN_PAGE) ;

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
                CNReleaseValue(vpool, *ptr) ;
        }
        CNFreeArrayData(&(vpool->arrayPool),
                         sizeof(struct CNValue *) * ELEMENT_NUM_IN_PAGE, elm) ;
}

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNDictionaryValue * dict = CNCastToDictionaryValue(val) ;
        struct CNList *list, *next ;
        for(list = dict->elementList ; list != NULL ; list = next){
                next = list->next ;
                releaseElements(vpool, list->data) ;
                CNFreeList(CNListPoolInValuePool(vpool), list) ;
        }
        dict->elementList = NULL ;
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

