/*
 * @file CNProgram.c
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNProgram.h>
#import <BasicKit/CNByteCode.h>

void
CNInitProgram(struct CNProgram * dst, struct CNValuePool * vpool)
{
        dst->valuePool = vpool ;
        CNInitValueList(&(dst->program), vpool) ;
}

void
CNDeinitProgram(struct CNProgram * dst)
{
        CNDeinitValueList(&(dst->program)) ;
}

void
CNDumpProgram(unsigned int indent, struct CNProgram * src)
{
        struct CNList * list = (src->program).firstItem ;
        for( ; list != NULL ; list = list->next) {
                CNDumpByteCode(indent, list->data) ;
        }
}

