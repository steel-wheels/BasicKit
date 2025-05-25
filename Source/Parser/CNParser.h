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

void
CNSetSourceCodeToLexicalParser(struct CNValueList * statementList) ;

void
CNSetCompilerToSyntaxParser(struct CNCompiler * compiler, struct CNValuePool * vpool) ;

unsigned int
CNGetCurrentParsingLine(void) ;

#endif /* CNParser_h */
