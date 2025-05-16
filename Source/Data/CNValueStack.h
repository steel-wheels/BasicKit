/*
 * @file CNValueStack.h
 * @description Define value stack data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNValueStack_h
#define CNValueStack_h

#import <BasicKit/CNValue.h>

struct CNValueStack {
        struct CNValuePool *    valuePool ;
        struct CNList *         valueList ;
} ;

static inline void
CNInitValueStack(struct CNValueStack * dst, struct CNValuePool * vpool)
{
        dst->valuePool  = vpool ;
        dst->valueList  = NULL ;
}

void
CNDeinitValueStack(struct CNValueStack * dst) ;

void
CNPushValueToStack(struct CNValueStack * dst, struct CNValue * src) ;

/* the return value will be NULL */
struct CNValue *
CNPopValueFromStack(struct CNValueStack * src) ;

#endif /* CNValueStack_h */
