/*
 * @file CNValueList.h
 * @description Define CNValueList data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUELIST_H
#define CNVALUELIST_H

#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>

struct CNValueList {
        struct CNValuePool *    valuePool ;
        struct CNList *         firstItem ;
} ;

static inline void
CNInitValueList(struct CNValueList * dst, struct CNValuePool * vpool)
{
        dst->valuePool = vpool ;
        dst->firstItem = NULL ;
}

void
CNDeinitValueList(struct CNValueList * dst) ;

static inline struct CNList *
CNFirstItemInValueList(struct CNValueList * src)
{
        return src->firstItem ;
}

void
CNAppendToValueList(struct CNValueList * dst, struct CNValue * src) ;

void
CNPrependToValueList(struct CNValueList * dst, struct CNValue * src) ;

struct CNValue *
CNPopFromValueList(struct CNValueList * dst) ;

void
CNPrintValueList(const struct CNValueList * src) ;

#endif /* CNVALUELIST_H */
