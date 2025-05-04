/*
 * @file ArisiaBasic-Yacc.h.h
 * @description Define functions to access ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef ArisiaBasic_Yacc_h
#define ArisiaBasic_Yacc_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNProgram.h>
#import <BasicKit/CNValueList.h>
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
CNInitParser(struct CNProgram * pdb) {
        extern void CNSetupParser(struct CNProgram * pdb) ; // defined in ArisiaBasic-Yacc.y
        extern void CNSetupLexer(struct CNValuePool * vpool) ;  // defined in ArisiaBasic-Lex.l
        CNSetupLexer(pdb->valuePool) ;
        CNSetupParser(pdb) ;
}

static inline unsigned int
CNExecParser(struct CNValueList * strings) {
        extern void CNStartParser(void) ;
        extern void CNSetSourceCode(struct CNValueList * strings) ;
        extern void CNDeinitSourceCode(void) ;
        extern unsigned int CNParserErrorCount(void) ;
        CNSetSourceCode(strings) ;
        CNStartParser() ;
        CNDeinitSourceCode() ;
        return CNParserErrorCount() ;
}

static inline void
CNFreeParser(void)
{
        extern void CNDeinitParser(void) ;
        CNDeinitParser() ;
}

#endif /* ArisiaBasic_Yacc_h */
