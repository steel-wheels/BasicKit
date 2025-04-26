/*
 * @file ArisiaBasic-Yacc.h.h
 * @description Define functions to access ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef ArisiaBasic_Yacc_h
#define ArisiaBasic_Yacc_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNValueList.h>
#import <BasicKit/CNIterators.h>
#import <BasicKit/CNInterface.h>

typedef union {
        struct CNValue * value ;
} CNToken ;

#define YYSTYPE CNToken

typedef struct {
        unsigned int    lineNumber ;
} CNSourceLocation ;

#define YYLTYPE CNSourceLocation

extern int yylex(void) ;

static inline void
CNInitParser(struct CNValuePool * vpool) {
        extern void CNSetupLexer(struct CNValuePool * vpool) ;  // defined in ArisiaBasic-Lex.l
        extern void CNSetupParser(struct CNValuePool * vpool) ; // defined in ArisiaBasic-Yacc.y
        CNSetupLexer(vpool) ;
        CNSetupParser(vpool) ;
}

static inline void
CNExecParser(struct CNValueList * strings) {
        extern void CNStartParser(void) ;
        extern void CNSetSourceCode(struct CNValueList * strings) ;
        extern void CNDeinitSourceCode(void) ;
        CNSetSourceCode(strings) ;
        CNStartParser() ;
        CNDeinitSourceCode() ;
}

#endif /* ArisiaBasic_Yacc_h */
