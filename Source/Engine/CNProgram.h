/*
 * @file CNProgram.h
 * @description Define CNProgtam data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNProgram_h
#define CNProgram_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNValueList.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNValue.h>

struct CNProgram {
        struct CNValuePool *    valuePool ;
        /* List of CNDictionary
         *   key:   CNString
         *   value: CNUnsignedInt
         */
        struct CNList *         variableTables ;
        /*
         * List of CNByteCode
         */
        struct CNValueList      program ;
} ;

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool) ;

void
CNDeinitProgram(struct CNProgram * dst) ;

static inline void
CNAppendCodeToProgram(struct CNProgram * dst, struct CNValue * opcode)
{
        CNAppendToValueList(&(dst->program), opcode) ;
}

void
CNDumpProgram(unsigned int indent, struct CNProgram * src) ;

#endif /* CNProgram_h */
