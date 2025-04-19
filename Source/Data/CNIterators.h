/*
 * @file CNIterators.h
 * @description Define iterators for each data
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNIterators_h
#define CNIterators_h

#import <BasicKit/CNValue.h>

struct CNStringIterator {
        struct CNValue *        sourceString ;
        struct CNValue *        currentString ;
        unsigned int            currentIndex ;
} ;

struct CNStringListIterator {
        struct CNList *         sourceStringList ;
        struct CNStringIterator stringIterator ;
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

void
CNInitStringListIterator(struct CNStringListIterator * dst, struct CNList * strings) ;

char
CNGetCharacterFromStringListIterator(struct CNStringListIterator * src) ;

#endif /* CNIterators_h */
