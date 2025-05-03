/*
 * @file CNProgram.c
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNProgram.h>
#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNValue.h>

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->uniqueRegisterId   = 0 ;
        CNInitRegisters(&(dst->registers), vpool) ;
        CNInitValueList(&(dst->program), vpool) ;
}

void
CNDeinitProgram(struct CNProgram * dst)
{
        CNDeinitRegisters(&(dst->registers)) ;
        CNDeinitValueList(&(dst->program)) ;
}

unsigned int
CNUniqueRegisterIdInProgram(struct CNProgram * src)
{
        unsigned int regid = src->uniqueRegisterId ;
        src->uniqueRegisterId += 1 ;
        return regid ;
}

void
CNDumpProgram(unsigned int indent, struct CNProgram * src)
{
        struct CNList * list = (src->program).firstItem ;
        for( ; list != NULL ; list = list->next) {
                CNDumpByteCode(indent, list->data) ;
        }
}

