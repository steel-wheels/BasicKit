/*
 * @file CNParseErrpr.h
 * @description Define CNParseError data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNParseError_h
#define CNParseError_h

#import <BasicKit/CNStringValue.h>

typedef enum {
        CNNoParseError                  = 0,
        CNSyntaxError,
        CNUndefinedVariableError,
        CNCanNotCastError
} CNParseErrorType ;

struct CNSyntaxError {
        struct CNStringValue *  message ;
} ;

struct CNUndefinedVariableError {
        struct CNStringValue *  identifier ;
} ;

struct CNCanNotCastError {
        CNValueType             destinationType ;
        CNValueType             sourceType ;
} ;

struct CNParseError {
        CNParseErrorType        type ;
        unsigned int            line ;
        union {
                struct CNSyntaxError                    syntaxError ;
                struct CNUndefinedVariableError         undefinedVariableError ;
                struct CNCanNotCastError                canNotCastError ;
        } ; // no name union
} ;

static inline struct CNParseError
CNMakeSyntaxError(struct CNStringValue * message, unsigned int line)
{
        CNRetainValue(CNSuperClassOfStringValue(message)) ;
        struct CNParseError result = {
                .type           = CNSyntaxError,
                .line           = line,
                .syntaxError = {
                        .message = message
                }
        } ;
        return result ;
}

static inline struct CNParseError
CNMakeUndefinedVariableError(struct CNStringValue * ident, unsigned int line)
{
        CNRetainValue(CNSuperClassOfStringValue(ident)) ;
        struct CNParseError result = {
                .type           = CNUndefinedVariableError,
                .line           = line,
                .undefinedVariableError = {
                        .identifier = ident
                }
        } ;
        return result ;
}

static inline struct CNParseError
CNMakeCanNotCastError(CNValueType dsttype, CNValueType srctype, unsigned int line)
{
        struct CNParseError result = {
                .type           = CNCanNotCastError,
                .line           = line,
                .canNotCastError = {
                        .destinationType        = dsttype,
                        .sourceType             = srctype
                }
        } ;
        return result ;
}

void
CNDeinitParseError(struct CNValuePool * vpool, struct CNParseError * dst) ;

void
CNPrintParseError(const struct CNParseError * src) ;

#endif /* CNParseError_h */
