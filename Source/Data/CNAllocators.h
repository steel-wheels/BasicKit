/*
 * @file CNAllocators.h
 * @description Define value allocator
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNAllocators_h
#define CNAllocators_h

#import <BasicKit/CNValueList.h>

void
CNInitStringList(struct CNValueList * dst, struct CNValuePool * vpool) ;

void
CNPutCharIntoStringList(struct CNValueList * dst, char c) ;

static inline void
CNPutStringIntoStringList(struct CNValueList * dst, const char * str) {
        for( ; *str != '\0' ; str++){
                CNPutCharIntoStringList(dst, *str) ;
        }
}

#endif /* CNAllocators_h */
