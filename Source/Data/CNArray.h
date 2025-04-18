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
        uint32_t                count ;
        struct CNValue **       values ;
} ;

int
CNCompareArray(const struct CNArray * s0, const struct CNArray * s1) ;

void
CNRetainArray(struct CNArray * dst) ;

void
CNReleaseArrayElements(struct CNValuePool * pool, struct CNArray * dst) ;

void
CNDeinitArray(struct CNValuePool * pool, struct CNArray * dst) ;

void
CNArrayDump(unsigned int indent, uint32_t count, const struct CNArray * src) ;

#endif /* CNARRAY_H */
