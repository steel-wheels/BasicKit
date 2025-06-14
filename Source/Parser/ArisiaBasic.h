/*
 * @file ArisiaBasic.h
 * @description Header file for ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef ArisiaBasic_h
#define ArisiaBasic_h

#include <BasicKit/CNValue.h>
#include <BasicKit/CNValueList.h>
#include <BasicKit/CNCompiler.h>
#include <BasicKit/CNInterface.h>
#include <stdint.h>

typedef void *  yyscan_t ;

typedef union {
        CNValueType             valueType ;
        struct CNVariable       variable ;
        struct CNStringValue *  identifier ;
        uint64_t                unsignedIntValue ;
        double                  floatValue ;
        struct CNStringValue *  string ;
} CNToken ;
#define YYSTYPE CNToken

struct _YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} ;

#define YYLTYPE struct _YYLTYPE

#define YYLEX_PARAM     &yylval, &yylloc

int yylex(YYSTYPE *lvalp, YYLTYPE *llocp);

#endif /* ArisiaBasic_h */
