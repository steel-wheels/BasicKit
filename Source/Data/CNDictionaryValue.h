/*
 * @file CNDictionaryValue.h
 * @description Define dictionary value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNDictionaryValue_h
#define CNDictionaryValue_h

#import <BasicKit/CNValue.h>
#import <BasicKit/CNStringValue.h>

struct CNDictionaryValue {
        struct CNValue                  superClass ;
        int64_t                         elementNum ;
        struct CNList *                 elementList ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForDictionaryValue(void) ;

struct CNDictionaryValue *
CNAllocateDictionaryValue(struct CNValuePool * vpool) ;

static inline struct CNDictionaryValue *
CNCastToDictionaryValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNDictionaryType ? (struct CNDictionaryValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfDictionaryValue(struct CNDictionaryValue * src)
{
        return &(src->superClass) ;
}

void
CNSetValueToDictionary(struct CNValuePool * vpool, struct CNDictionaryValue * dst,
                       struct CNStringValue * key, struct CNValue * value) ;

struct CNValue *
CNValueForKeyInDictionary(struct CNDictionaryValue * dst, struct CNStringValue * key) ;

#endif /* CNDictionaryValue_h */
