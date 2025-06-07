/*
 * @file CNOperation.c
 * @description Define CNByteCode executer
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNOperation.h"

const char *
CNArithmeticOperationName(CNNumberBinaryOperation op)
{
        const char * result = "?" ;
        switch(op){
                case CNAddOperation:            result = "+" ;   break ;
                case CNSubOperation:            result = "-" ;   break ;
                case CNMultOperation:           result = "*" ;   break ;
                case CNDivFloatOperation:       result = "/" ;   break ;
                case CNDivIntOperation:         result = "div" ; break ;
                case CNModOperation:            result = "mod" ; break ;
        }
        return result ;
}
