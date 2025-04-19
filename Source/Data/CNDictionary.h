/*
 * @file CNDuctionary.h
 * @description Define CNDictionary data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNDictionary_h
#define CNDictionary_h

#import <BasicKit/CNType.h>

#define CNDICTIONARY_ELEMENT_NUM       3

struct CNDictionaryElement {
        struct CNValue *        key ;   /* string */
        struct CNValue *        value ;
} ;

struct CNDictionary {
        struct CNValue *                next ;
        struct CNDictionaryElement      elements[CNDICTIONARY_ELEMENT_NUM] ;
} ;

void
CNSetKeyAndValueToDictionary(struct CNDictionary * dst, struct CNValue * key, struct CNValue * value, struct CNValuePool * pool) ;

struct CNValue *
CNSearchValueInDictionary(const struct CNDictionary * src, const struct CNValue * key) ;

int
CNCompareDictionary(const struct CNDictionary * s0, const struct CNDictionary * s1) ;

void
CNRetainDictionary(struct CNDictionary * dst) ;

void
CNReleaseDictionaryElements(struct CNValuePool * pool, struct CNDictionary * dst) ;

static inline void
CNDeinitDictionary(struct CNValuePool * pool, struct CNDictionary * dst)
{
        /* do nothing */
        (void) pool ; (void) dst ;
}

void
CNDumpDictionary(unsigned int indent, const struct CNDictionary * src) ;

#endif /* CNDictionary_h */
