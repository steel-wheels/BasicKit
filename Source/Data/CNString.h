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

int
CNCompareString(uint32_t len0, const struct CNString * s0, uint32_t len1, const struct CNString * s1) ;

void
CNRetainString(struct CNString * dst) ;

void
CNReleaseNextString(struct CNValuePool * pool, struct CNString * dst) ;

static inline void
CNDeinitString(struct CNValuePool * pool, struct CNString * dst)
{
        /* Do nothing */
        (void) pool ; (void) dst ;
}

void
CNDumpString(uint32_t count, const struct CNString * src) ;

#endif /* CNSTRING_H */
