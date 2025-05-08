/*
 * @file CNProgram.h
 * @description Define CNProgtam data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNProgram_h
#define CNProgram_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNRegisters.h>
#import <BasicKit/CNValueList.h>
#import <BasicKit/CNValuePool.h>

struct CNProgram {
        struct CNValuePool *    valuePool ;
        struct CNRegisters      registers ;
        struct CNValue *        identifierTable ;       // Dictionary
        struct CNValueList      program ;               // List of CNByteCode
} ;

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool) ;

void
CNDeinitProgram(struct CNProgram * dst) ;

static inline uint64_t
CNAllocateRegisterInProgram(struct CNProgram * src)
{
        return CNAllocateRegisterInRegisters(&(src->registers)) ;
}

bool
CNHasRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * identstr) ;

uint64_t
CNRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * identstr) ;

static inline void
CNAppendCodeToProgram(struct CNProgram * dst, struct CNValue * opcode)
{
        CNAppendToValueList(&(dst->program), opcode) ;
}

void
CNDumpProgram(unsigned int indent, struct CNProgram * src) ;

#endif /* CNProgram_h */
