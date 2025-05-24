/*
 * @file CNParser.h
 * @description Define function for ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNParser_h
#define CNParser_h

#include <BasicKit/CNValuePool.h>
#include <BasicKit/CNCompiler.h>

void
CNInitLexicalParser(struct CNValuePool * vpool) ;

void
CNDeinitLexicalParser(void) ;

void
CNStartParser(void) ;

/*
 * progtam: List of CNStringValue
 */
void
CNSetSourceCodeToLexicalParser(struct CNValueList * statementList) ;

void
CNSetCompilerToSyntaxParser(struct CNCompiler * compiler, struct CNValuePool * vpool) ;

#endif /* CNParser_h */
