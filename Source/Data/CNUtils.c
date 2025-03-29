/*
 * @file CNUtils.c
 * @description Define utility functions
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNUtils.h"
#include <stdio.h>

void
CNDumpIndent(unsigned int indent)
{
        unsigned int i ;
        for(i=0 ; i<indent ; i++){
                fputs("  ", stdout) ;
        }
}

