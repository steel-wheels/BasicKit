%{

/* ArisiaBasic-Yacc.y */

#include "ArisiaBasic.h"
#include "CNByteCode.h"
#include "CNList.h"

static struct CNParserDB *      s_parser_db   = NULL ;
static unsigned int             s_uniq_reg_id = 0 ;

void
CNSetupParser(struct CNParserDB * pdb)
{
        s_parser_db   = pdb ;
}

void
CNDeinitParser(void)
{
        s_parser_db   = NULL ;
}

static inline unsigned int
uniqueRegId(void)
{
        unsigned int regid = s_uniq_reg_id ;
        s_uniq_reg_id += 1 ;
        return regid ;
}

static struct CNValue *
registerId(unsigned int regid)
{
        return CNAllocateUnsignedInt(regid, s_parser_db->valuePool) ;
}

static struct CNValue *
storeString(struct CNValue * dstreg, struct CNValue * srcstr)
{
        struct CNValue * opcode = CNAllocateStoreStringByteCode(
                s_parser_db->valuePool, dstreg, srcstr) ;
        return opcode ;
}

static inline void
appendToBlock(struct CNValue * opcode)
{
        CNAppendCodeToProgram(s_parser_db, opcode) ;
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
                CNReleaseValue(s_parser_db->valuePool, $2.value) ;
        }
        ;

expression
        : STRING
        {
                struct CNValue * regid  = registerId(uniqueRegId()) ;
                struct CNValue * opcode = storeString(regid, $1.value) ;
                appendToBlock(opcode) ;
                CNReleaseValue(s_parser_db->valuePool, $1.value) ;
                CNReleaseValue(s_parser_db->valuePool, opcode) ;
                $0.value = regid ;
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

