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
registerId(unsigned int regid)
{
        return CNAllocateUnsignedInt(regid, s_program->valuePool) ;
}

static struct CNValue *
moveCode(struct CNValue * dstreg, struct CNValue * srcreg)
{
        struct CNValue * opcode = CNAllocateMoveByteCode(s_program->valuePool, dstreg, srcreg) ;
        return opcode ;
}

static struct CNValue *
storeCode(struct CNValue * dstreg, struct CNValue * srcval)
{
        struct CNValue * opcode = CNAllocateStoreByteCode(s_program->valuePool, dstreg, srcval) ;
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

static unsigned int
currentLineNumber(void)
{
        return 0 ;
}

static int yyerror(char const * str) ;

%}

/* reserved words */
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

statement
        : IDENTIFIER '=' expression
          {
                struct CNValue * identstr = $1.value ;  // string
                struct CNValue * srcid    = $3.value ;  // unsigned int

                struct CNValue * dstid ;
                if((dstid = CNRegisterIdForIdentifier(s_program, identstr)) == NULL){
                        /* the identifier is not found */
                        dstid = CNAllocateRegisterIdForIdentifier(s_program, identstr) ;
                }
                struct CNValue * opcode = moveCode(dstid, srcid) ;
                appendToBlock(opcode) ;

                CNReleaseValue(s_program->valuePool, identstr) ;
                CNReleaseValue(s_program->valuePool, dstid) ;
                CNReleaseValue(s_program->valuePool, srcid) ;
                CNReleaseValue(s_program->valuePool, opcode) ;
          }
        | PRINT expression
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
                struct CNValue * opcode = storeCode(regval, $1.value) ;
                appendToBlock(opcode) ;
                CNReleaseValue(s_program->valuePool, $1.value) ;
                CNReleaseValue(s_program->valuePool, opcode) ;
                $$.value = regval ;
          }
        | IDENTIFIER
        {
                struct CNValue * identstr = $1.value ;  // string

                struct CNValue * srcid ;
                if((srcid = CNRegisterIdForIdentifier(s_program, identstr)) == NULL){
                        unsigned int lineno = currentLineNumber() ;

                        CNInterface()->printf("Variable ") ;
                        CNPrintValue(identstr) ;
                        CNInterface()->printf(" is refered at line %u\n", lineno) ;
                        s_error_count += 1 ;

                        /* the identifier is not found */
                        srcid = CNAllocateRegisterIdForIdentifier(s_program, identstr) ;
                }
                $$.value = srcid ;
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

