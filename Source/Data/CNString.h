/*
 * @file CNString.h
 * @description Define CNString data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNSTRING_H
#define CNSTRING_H

#import <BasicKit/CNType.h>

#define CNSTRING_ELEMENT_NUM            48

struct CNString {
        struct CNValue *        next ;
        char                    buffer[CNSTRING_ELEMENT_NUM] ;
} ;

void
CNRetainString(struct CNString * dst) ;

void
CNReleaseString(struct CNValuePool * pool, struct CNString * dst) ;

void
CNDumpString(uint32_t count, const struct CNString * src) ;

#endif /* CNSTRING_H */
