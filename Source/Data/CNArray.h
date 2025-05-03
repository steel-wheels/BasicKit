/*
 * @file CNArray.h
 * @description Define CNArray data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNARRAY_H
#define CNARRAY_H

#import <BasicKit/CNType.h>

struct CNArray {
        uint64_t                count ;
        struct CNValue **       values ;
} ;

int
CNCompareArray(const struct CNArray * s0, const struct CNArray * s1) ;

static inline struct CNValue *
CNElementInArray(struct CNArray * src, uint64_t index)
{
        return (src->values)[index] ;
}

void
CNSetElementToArray(struct CNValuePool * cpool, struct CNArray * dst, uint64_t index, struct CNValue * newval) ;

void
CNRetainArray(struct CNArray * dst) ;

void
CNReleaseArrayElements(struct CNValuePool * pool, struct CNArray * dst) ;

void
CNDeinitArray(struct CNValuePool * pool, struct CNArray * dst) ;

void
CNPrintArray(uint32_t elmnum, const struct CNArray * src) ;

#endif /* CNARRAY_H */
