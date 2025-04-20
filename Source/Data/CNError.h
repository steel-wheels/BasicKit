/*
 * @file CNError.h
 * @description Define CNError data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNError_h
#define CNError_h

#include <stdio.h>

typedef enum {
        CNNoError
} CNErrorCode ;

struct CNError {
        CNErrorCode             errorCode ;
} ;

const char *
CNErrorCodeToString(CNErrorCode ecode) ;

static inline void
CNReleaseError(const struct CNError * dst)
{
        /* nothing have to do */
        (void) dst ;
}

static inline void
CNRetainError(struct CNError * src)
{
        /* nothing have to do */
        (void) src ;
}

static inline void
CNDeinitError(struct CNError * src)
{
        /* nothing have to do */
        (void) src ;
}

int
CNCompareError(const struct CNError * e0, const struct CNError * e1) ;

void
CNDumpError(unsigned int indent, const struct CNError * src) ;

#endif /* CNError_h */
