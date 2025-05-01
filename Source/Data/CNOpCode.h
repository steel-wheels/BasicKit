/*
 * @file CNOpCode.h
 * @description Define CNOpCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNOpCode_h
#define CNOpCode_h

#import <BasicKit/CNType.h>

struct CNOpCode {
        uint64_t                attribute ;
        struct CNList *         children ;      // list of CNOpCode, will be null
        struct CNValue *        destination ;
        struct CNValue *        source0 ;
        struct CNValue *        source1 ;
} ;

void
CNRetainOpCode(struct CNOpCode * dst) ;

void
CNReleaseOpCode(struct CNValuePool * pool, struct CNOpCode * dst) ;

static inline void
CNDeinitOpCode(struct CNValuePool * pool, struct CNOpCode * dst)
{
        /* do nothing */
        (void) pool ; (void) dst ;
}

int
CNCompareOpCode(const struct CNOpCode * s0, const struct CNOpCode * s1) ;

void
CNDumpOpCode(uint32_t indent, const struct CNOpCode * src) ;

#endif /* CNOpCode_h */
