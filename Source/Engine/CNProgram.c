/*
 * @file CNProgram.c
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNProgram.h>
#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNInterface.h>

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->identifierTable    = CNAllocateDictionary(vpool) ;
        CNInitRegisters(&(dst->registers), vpool) ;
        CNInitValueList(&(dst->program), vpool) ;
}

void
CNDeinitProgram(struct CNProgram * dst)
{
        CNReleaseValue(dst->valuePool, dst->identifierTable) ;
        CNDeinitRegisters(&(dst->registers)) ;
        CNDeinitValueList(&(dst->program)) ;
}

struct CNValue *
CNRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * identstr)
{
        struct CNDictionary * table = &((src->identifierTable)->dictionaryValue) ;
        return CNSearchValueInDictionary(table, identstr) ;
}

struct CNValue *
CNAllocateRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * identstr)
{
        uint64_t         newregid  = CNAllocateRegisterInProgram(src) ;
        struct CNValue * newregval = CNAllocateUnsignedInt(newregid, src->valuePool) ;

        struct CNDictionary * table = &((src->identifierTable)->dictionaryValue) ;
        CNSetKeyAndValueToDictionary(table, identstr, newregval, src->valuePool) ;

        return newregval ;
}

void
CNDumpProgram(unsigned int indent, struct CNProgram * src)
{
        struct CNList * list = (src->program).firstItem ;
        for( ; list != NULL ; list = list->next) {
                _CNPrintByteCode(list->data) ;
                CNInterface()->printf("\n") ;
        }
}

