%{

/* ArisiaBasic-Yacc.y */

#include "ArisiaBasic.h"
#include "CNByteCode.h"
#include "CNList.h"

static struct CNProgram *       s_program     = NULL ;
static unsigned int             s_error_count = 0 ;

void
CNSetupParser(struct CNProgram * prg)
{
        s_program     = prg ;
        s_error_count = 0 ;
}

void
CNDeinitParser(void)
{
        s_program       = NULL ;
        s_error_count   = 0 ;
}

unsigned int
CNParserErrorCount(void)
{
        return s_error_count ;
}

static struct CNValue *
moveCode(uint64_t dstreg, uint64_t srcreg)
{
        return CNAllocateMoveByteCode(s_program->valuePool, dstreg, srcreg) ;
}

static struct CNValue *
storeCode(uint64_t dstreg, struct CNValue * srcval)
{
        return CNAllocateStoreByteCode(s_program->valuePool, dstreg, srcval) ;
}

static struct CNValue *
printCode(uint64_t srcregid)
{
        return CNAllocatePrintByteCode(s_program->valuePool, srcregid) ;
}

static inline void
appendToBlock(struct CNValue * opcode)
{
        CNAppendCodeToProgram(s_program, opcode) ;
}

static unsigned int
currentLineNumber(void)
{
        return 0 ;
}

static int yyerror(char const * str) ;

%}

/* reserved words */
%token  _TRUE
%token  _FALSE
%token  LET
%token  PRINT

%token  IDENTIFIER
%token  STRING

%start statement_list

%%

statement_list
        : statement_list statement
        | statement
        ;

statement // has no value
        : IDENTIFIER '=' expression
        {
                struct CNValue * identstr = $1.value ;
                uint64_t         srcreg   = $3.registerId ;

                uint64_t dstreg = CNRegisterIdForIdentifier(s_program, identstr) ;

                struct CNValue * opcode = moveCode(dstreg, srcreg) ;
                appendToBlock(opcode) ;

                CNReleaseValue(s_program->valuePool, identstr) ;
                CNReleaseValue(s_program->valuePool, opcode) ;
        }
        | PRINT expression
        {
                uint64_t srcreg = $2.registerId ;
                struct CNValue * opcode = printCode(srcreg) ;
                appendToBlock(opcode) ;

                CNReleaseValue(s_program->valuePool, opcode) ;
        }
        ;

expression // has registerId
        : boolean_expression
        {
                $$.registerId = $1.registerId ;
        }
        | STRING
        {
                uint64_t         dstreg = CNAllocateRegisterInProgram(s_program) ;
                struct CNValue * srcval = $1.value ;
                struct CNValue * opcode = storeCode(dstreg, srcval) ;
                appendToBlock(opcode) ;
                CNReleaseValue(s_program->valuePool, srcval) ;
                CNReleaseValue(s_program->valuePool, opcode) ;

                $$.registerId = dstreg ;
        }
        | IDENTIFIER
        {
                struct CNValue * identstr = $1.value ;  // string
                uint64_t srcreg = CNRegisterIdForIdentifier(s_program, identstr) ;
                $$.registerId = srcreg ;
        }
        ;

boolean_expression // has registerId
        : _TRUE
          {
                  $$.registerId = (uint64_t) CNTrueValueRegister ;
          }
        | _FALSE
          {
                  $$.registerId = (uint64_t) CNFalseValueRegister ;
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
        s_error_count += 1 ;
        return 0 ;
}

