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
        dst->valuePool = NULL ;
}

bool
CNHasRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * ident)
{
        struct CNDictionary * table = &((src->identifierTable)->dictionaryValue) ;
        if(CNSearchValueInDictionary(table, ident) != NULL){
                return true ;
        } else {
                return false ;
        }
}

uint64_t
CNRegisterIdForIdentifier(struct CNProgram * src, struct CNValue * ident)
{
        struct CNDictionary * table = &((src->identifierTable)->dictionaryValue) ;

        struct CNValue * regval ;
        if((regval = CNSearchValueInDictionary(table, ident)) != NULL){
                return CNUnsignedIntValue(regval) ;
        } else {
                uint64_t regid = CNAllocateRegisterInProgram(src) ;
                struct CNValue * regval = CNAllocateUnsignedInt(regid, src->valuePool) ;
                CNSetKeyAndValueToDictionary(table, ident, regval, src->valuePool) ;
                CNReleaseValue(src->valuePool, regval) ;
                return regid ;
        }
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

