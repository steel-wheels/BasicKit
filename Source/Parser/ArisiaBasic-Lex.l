%{
#include "ArisiaBasic.h"
#include "CNStringValue.h"
#include "CNStringIterator.h"
#include "y.tab.h"

static int
getChar(void) ;

static struct CNValuePool *     s_value_pool ;
static struct CNStringIterator  s_iterator ;

void
CNInitLexicalParser(struct CNValuePool * vpool)
{
        s_value_pool = vpool ;
        CNInitStringIterator(&s_iterator, vpool) ;
}

void
CNSetProgramToLexicalParser(struct CNValueList * statementList)
{
        CNSetStringListToStringIterator(&s_iterator, statementList) ;
}

static bool
isReservedWord(int * rwordid, const char * str) ;

#define YY_INPUT(buf,result,max_size) \
    { \
        int c = getChar(); \
        result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

%}

%option bison-bridge bison-locations
%option noyywrap

SPACE           [ \t]
IDENTIFIER      [a-zA-Z][_0-9a-zA-Z]*

%%

{SPACE}+
;

{IDENTIFIER}  {
        int rwordid ;
        if(isReservedWord(&rwordid, yytext)){
                yylval->registerId = 0 ;
                return rwordid ;
        } else {
                const char * str = yytext ;
                uint32_t len = (uint32_t) strlen(str) ;
                yylval->identifier = CNAllocateStringValue(s_value_pool, len, str) ;
                return IDENTIFIER ;
        }
}

%%

static int
getChar(void)
{
        return CNGetCharFromStringItetator(&s_iterator) ;
}

static bool
isReservedWord(int * rwordid, const char * str)
{
        struct RWord {
                int             rId ;
                const char *    rWord ;
        } ;
        const struct RWord words[] = {
                { .rId = LET,    .rWord = "let"         },
                { .rId = PRINT,  .rWord = "print"       },
                { .rId = _FALSE, .rWord = "false"       },
                { .rId = _TRUE,  .rWord = "true"        },
                { .rId = -1,     .rWord = ""            }  // Terminal word
        } ;
        for(const struct RWord * word = words ; word->rId != -1 ; word++){
                if(strcasecmp(str, word->rWord) == 0){
                        *rwordid = word->rId ;
                        return true ;
                }
        }
        return false ;
}

