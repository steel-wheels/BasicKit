/*
 * @file CNStringIterator.h
 * @description Define function to iterate string list
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNStringIterator_h
#define CNStringIterator_h

#import <BasicKit/CNValueList.h>
#import <BasicKit/CNStringValue.h>

struct CNStringIterator {
        struct CNValuePool *    valuePool ;
        struct CNValueList      stringList ;
        struct CNStringValue *  currentString ;
        unsigned int            currentIndex ;
} ;

void
CNInitStringIterator(struct CNStringIterator * dst, struct CNValueList * strlist,
                     struct CNValuePool * pool) ;

int
CNGetCharFromStringItetator(struct CNStringIterator * src) ;

void
CNDeinitStringIterator(struct CNStringIterator * dst) ;

#endif /* CNStringIterator_h */
