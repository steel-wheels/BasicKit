/*
 * @file CNProgtam.c
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNProgram.h>
#include <stdio.h>

static struct CNProgram *       s_current_program = NULL ;

struct CNProgram *
CNCurrentProgram(void)
{
        return s_current_program ;
}

char
CNGetCharFromProgram(void)
{
        struct CNProgram * program = CNCurrentProgram() ;
        if(program != NULL){
                return CNGetCharacterFromStringListIterator(&(program->iterator)) ;
        } else {
                return EOF ;
        }
}
