/*
 * @file CNValueList.h
 * @description Define value list data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNValueList_h
#define CNValueList_h

#import <BasicKit/CNValue.h>
#import <BasicKit/CNList.h>

struct CNValueList {
        struct CNValuePool *    valuePool ;
        struct CNList *         valueList ;
} ;

static inline void
CNInitValueList(struct CNValueList * dst, struct CNValuePool * vpool)
{
        dst->valuePool  = vpool ;
        dst->valueList  = NULL ;
}

void
CNDeinitValueList(struct CNValueList * dst) ;

static inline struct CNList *
CNListInValueList(struct CNValueList * src)
{
        return src->valueList ;
}

unsigned int
CNNumberOfValuesInValueList(const struct CNValueList * src) ;

void
CNAppendValueToValueList(struct CNValueList * dst, struct CNValue * src) ;

void
CNPushValueToValueList(struct CNValueList * dst, struct CNValue * src) ;

struct CNValue *
CNPopValueFromValueList(struct CNValueList * dst) ;

void
CNPrintValueList(struct CNValueList * src) ;

#endif /* CNValueList_h */
