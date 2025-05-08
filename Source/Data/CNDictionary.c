/*
 * @file CNDuctionary.c
 * @description Define CNDictionary data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNDictionary.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

static unsigned int
CNNumberOfElementsInDictionary(const struct CNDictionary * src) ;

static bool
CNReplaceDictionaryValueByKey(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value,struct CNValuePool * pool) ;
static void
CNAddDictionaryValueByKey(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value,struct CNValuePool * pool) ;

static inline bool
hasValidContextInElement(const struct CNDictionaryElement * src)
{
        return CNTypeOfValue(src->key) != CNNullType ;
}

static unsigned int
CNNumberOfElementsInDictionary(const struct CNDictionary * src)
{
        unsigned int result = 0 ;
        while(src != NULL){
                for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                        const struct CNDictionaryElement * elm = &(src->elements[i]) ;
                        if(hasValidContextInElement(elm)){
                                result += 1 ;
                        }
                }
                struct CNValue * next = src->next ;
                src = next != NULL ? &(next->dictionaryValue) : NULL ;
        }
        return result ;
}
void
CNSetKeyAndValueToDictionary(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value,struct CNValuePool * pool)
{
        if(!CNReplaceDictionaryValueByKey(dst, key, value, pool)){
                CNAddDictionaryValueByKey(dst, key, value, pool) ;
        }
}

static bool
CNReplaceDictionaryValueByKey(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value,struct CNValuePool * pool)
{
        while(dst != NULL){
                for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                        struct CNDictionaryElement * elm = &(dst->elements[i]) ;
                        if(hasValidContextInElement(elm)){
                                if(CNCompareValue(elm->key, key) == 0){
                                        CNRetainValue(value) ;
                                        CNReleaseValue(pool, elm->value) ;
                                        elm->value = value ;
                                        return true ;
                                }
                        }
                }
                struct CNValue * next = dst->next ;
                dst = next != NULL ? &(next->dictionaryValue) : NULL ;
        }
        return false ;
}

static void
CNAddDictionaryValueByKey(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value,struct CNValuePool * pool)
{
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                struct CNDictionaryElement * elm = &(dst->elements[i]) ;
                if(!hasValidContextInElement(elm)) {
                        elm->key   = key ;      CNRetainValue(key) ;
                        elm->value = value ;    CNRetainValue(value) ;
                        return ;
                }
        }
        if(dst->next != NULL){
                CNAddDictionaryValueByKey(&(dst->next->dictionaryValue), key, value, pool) ;
        } else {
                struct CNValue * newdict = CNAllocateDictionary(pool) ;
                dst->next = newdict ;
                CNAddDictionaryValueByKey(&(newdict->dictionaryValue), key, value, pool) ;
        }
}

struct CNValue *
CNSearchValueInDictionary(const struct CNDictionary * src, const struct CNValue * key)
{
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                const struct CNDictionaryElement * elm = &(src->elements[i]) ;
                if(hasValidContextInElement(elm)) {
                        if(CNCompareValue(elm->key, key) == 0){
                                return elm->value ;
                        }
                }
        }
        struct CNValue * next = src->next ;
        if(src->next != NULL){
                struct CNDictionary * dict = &(next->dictionaryValue) ;
                return CNSearchValueInDictionary(dict, key) ;
        } else {
                return NULL ;
        }
}

int
CNCompareDictionary(const struct CNDictionary * s0, const struct CNDictionary * s1)
{
        unsigned int n0 = CNNumberOfElementsInDictionary(s0) ;
        unsigned int n1 = CNNumberOfElementsInDictionary(s1) ;
        if(n0 > n1){
                return 1 ;
        } else if(n0 < n1){
                return -1 ;
        } else {
                for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                        const struct CNDictionaryElement * elm0 = &(s0->elements[i]) ;
                        if(hasValidContextInElement(elm0)) {
                                struct CNValue * val1 ;
                                val1 = CNSearchValueInDictionary(s1, elm0->key) ;
                                if(val1 != NULL){
                                        unsigned int res = CNCompareValue(elm0->value, val1) ;
                                        if(res != 0){
                                                return res ;
                                        }
                                } else {
                                        return 1 ;
                                }
                        }
                }
                return 0 ;
        }
}

void
CNRetainDictionary(struct CNDictionary * dst)
{
        struct CNValue * next ;
        if((next = dst->next) != NULL){
                CNRetainValue(next) ;
        }
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                struct CNDictionaryElement * elm = &(dst->elements[i]) ;
                CNRetainValue(elm->key) ;
                CNRetainValue(elm->value) ;
        }
}

void
CNReleaseDictionaryElements(struct CNValuePool * pool, struct CNDictionary * dst)
{
        struct CNValue * next ;
        if((next = dst->next) != NULL){
                CNReleaseValue(pool, next) ;
        }
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                struct CNDictionaryElement * elm = &(dst->elements[i]) ;
                CNReleaseValue(pool, elm->key) ;
                CNReleaseValue(pool, elm->value) ;
        }
}

void
CNPrintDictionary(const struct CNDictionary * src)
{
        for(unsigned int i=0 ; i < CNDICTIONARY_ELEMENT_NUM ; i++){
                const struct CNDictionaryElement * elm = &(src->elements[i]) ;
                if(elm->key != NULL && elm->value) {
                        CNPrintValue(elm->key) ;
                        CNInterface()->printf(":") ;
                        CNPrintValue(elm->value) ;
                }

        }
        const struct CNValue * next ;
        if((next = src->next) != NULL) {
                CNPrintDictionary(&(next->dictionaryValue)) ;
        }
}
