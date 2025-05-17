/*
 * @file ArisiaBasic.h
 * @description Header file for ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef ArisiaBasic_h
#define ArisiaBasic_h

#include <BasicKit/CNInterface.h>
#include <stdint.h>

typedef void *  yyscan_t ;

typedef union {
        uint64_t                registerId ;
        struct CNValue *        value ;
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
