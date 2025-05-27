/*
 * @file CNRegisterFile.h
 * @description Define CNRegisterFile data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNRegisterFile_h
#define CNRegisterFile_h

#import <BasicKit/CNArrayValue.h>
#import <BasicKit/CNValuePool.h>

#define CNRegisterPageNum       8
#define CNRegisterNumInPage     1024

struct CNRegisterFile {
        struct CNValuePool *    valuePool ;
        index_t                 currentIndex ;
        struct CNArrayValue *   registerPages[CNRegisterPageNum] ;
} ;

void
CNInitRegisterFile(struct CNRegisterFile * dst, struct CNValuePool * vpool) ;

void
CNDeinitRegisterFile(struct CNRegisterFile * dst) ;

static inline index_t
CNGetFreeRegisterIdInRegisterFile(struct CNRegisterFile * src)
{
        index_t idx = src->currentIndex ;
        src->currentIndex += 1 ;
        return idx ;
}

void
CNSetValueToRegisterFile(struct CNRegisterFile * src, index_t idx, struct CNValue * val) ;

/* The return value will be NULL */
struct CNValue *
CNValueInRegisterFile(struct CNRegisterFile * src, index_t idx) ;

#endif /* CNRegisterFile_h */
