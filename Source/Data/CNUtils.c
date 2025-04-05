/*
 * @file CNUtils.c
 * @description Define utility functions
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNInterface.h>
#include "CNUtils.h"

#define MIN(A, B)       ((A) < (B) ? (A) : (B))
void
CNDumpIndent(unsigned int indent)
{
#       define MAX_INDENT       80
        char spaces[MAX_INDENT + 1] ;
        unsigned int i ;
        unsigned int maxidt = MIN(indent, MAX_INDENT) ;
        for(i=0 ; i<maxidt ; i++){
                spaces[i] = ' ' ;
        }
        spaces[i] = '\0' ;
        CNInterface()->printf(spaces) ;
}

