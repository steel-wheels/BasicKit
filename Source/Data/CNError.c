/*
 * @file CNError.c
 * @description Define CNError data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNError.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

const char *
CNErrorCodeToString(CNErrorCode ecode)
{
        const char * result = "?" ;
        switch(ecode){
                case CNNoError: {
                        result = "No error" ;
                } break ;
        }
        return result ;
}

int
CNCompareError(const struct CNError * e0, const struct CNError * e1)
{
        return e0->errorCode - e1->errorCode ;
}

void
CNDumpError(unsigned int indent, const struct CNError * src)
{
        CNDumpIndent(indent) ;
        const char * estr = CNErrorCodeToString(src->errorCode) ;
        CNInterface()->printf("error: { code:0x%x, message: \"%s\"}\n",
                              src->errorCode, estr) ;
}

