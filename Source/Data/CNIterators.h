/*
 * @file CNIterators.h
 * @description Define iterators for each data
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNIterators_h
#define CNIterators_h

#import <BasicKit/CNValue.h>
#import <BasicKit/CNValueList.h>

struct CNStringIterator {
        struct CNValue *        sourceString ;
        struct CNValue *        currentString ;
        unsigned int            currentIndex ;
} ;

void
CNInitStringIterator(struct CNStringIterator * dst, struct CNValue * string) ;

static inline void
CNDeinitStringIterator(struct CNValuePool * pool, struct CNStringIterator * dst)
{
        struct CNValue * srcstr = dst->sourceString ;
        if(srcstr != NULL){
                CNReleaseValue(pool, srcstr) ;
        }
}

char
CNGetCharacterFromStringIterator(struct CNStringIterator * src) ;

struct CNStringListIterator {
        struct CNValueList *    stringList ;
        struct CNValuePool *    valuePool ;
        struct CNStringIterator stringIterator ;
} ;

void
CNInitStringListIterator(struct CNStringListIterator * dst, struct CNValuePool * vpool,
                         struct CNValueList * strings) ;

static inline void
CNDeinitStringListIterator(struct CNStringListIterator * dst)
{
        CNDeinitStringIterator(dst->valuePool, &(dst->stringIterator)) ;
}

char
CNGetCharacterFromStringListIterator(struct CNStringListIterator * src) ;

#endif /* CNIterators_h */
