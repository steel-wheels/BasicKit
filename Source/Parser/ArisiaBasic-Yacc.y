%{

/* ArisiaBasic-Yacc.y */

#include "ArisiaBasic.h"
#include "CNByteCode.h"
#include "CNList.h"

static struct CNProgram *      s_program     = NULL ;

void
CNSetupParser(struct CNProgram * prg)
{
        s_program = prg ;
}

void
CNDeinitParser(void)
{
        s_program = NULL ;
}

static struct CNValue *
registerId(unsigned int regid)
{
        return CNAllocateUnsignedInt(regid, s_program->valuePool) ;
}

static struct CNValue *
storeStringCode(struct CNValue * dstreg, struct CNValue * srcstr)
{
        struct CNValue * opcode = CNAllocateStoreStringByteCode(
                s_program->valuePool, dstreg, srcstr) ;
        return opcode ;
}

static struct CNValue *
printCode(struct CNValue * srcregid)
{
        struct CNValue * opcode = CNAllocatePrintByteCode(
                s_program->valuePool, srcregid) ;
        return opcode ;
}

static inline void
appendToBlock(struct CNValue * opcode)
{
        CNAppendCodeToProgram(s_program, opcode) ;
}

static int yyerror(char const * str) ;

%}

/* reserved words */
%token  LET
%token  PRINT

%token  IDENTIFIER
%token  STRING

%start statement

%%

statement
        : PRINT expression
        {
                struct CNValue * opcode = printCode($2.value) ;
                appendToBlock(opcode) ;
                CNReleaseValue(s_program->valuePool, $2.value) ;
                CNReleaseValue(s_program->valuePool, opcode) ;
        }
        ;

expression
        : STRING
        {
                unsigned int regid= CNUniqueRegisterIdInProgram(s_program) ;
                struct CNValue * regval = registerId(regid) ;
                struct CNValue * opcode = storeStringCode(regval, $1.value) ;
                appendToBlock(opcode) ;
                CNReleaseValue(s_program->valuePool, $1.value) ;
                CNReleaseValue(s_program->valuePool, opcode) ;
                $$.value = regval ;
        }
        ;

%%

void CNStartParser(void)
{
        yyparse() ;
}

static int
yyerror(char const *str)
{
        CNInterface()->error(str) ;
        CNInterface()->error("\n") ;
        return 0 ;
}

