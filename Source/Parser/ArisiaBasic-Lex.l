%{

/* ArisiaBasic-Lex.l */
#include "ArisiaBasic.h"
#include "y.tab.h"      // generated from ArisiaBasic.y
#include "CNValue.h"
#include <stdbool.h>

static struct CNValuePool *     s_valuePool = NULL ;
static struct CNStringListIterator s_string_reader ;
static int getChar(void) ;

void
CNSetupLexer(struct CNValuePool * vpool)
{
        s_valuePool = vpool ;
}

static bool
isReservedWord(int * rwordid, const char * str) ;

#define YY_INPUT(buf,result,max_size) \
    { \
        int c = getChar(); \
        result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }
    
%}

%option noyywrap

SPACE          [ \t]
NEWLINE        \n
IDENT           [a-zA-Z][_0-9a-zA-Z]*

%x CSTRING

%%
{SPACE}+

{IDENT}  {
        int rwordid ;
        if(isReservedWord(&rwordid, yytext)){
                yylval.value = CNAllocateNull() ;
                return rwordid ;
        } else {
                const char * str = yytext ;
                uint32_t     len = (uint32_t) strlen(str) ;
                yylval.value = CNAllocateString(str, len, s_valuePool) ;
                return IDENTIFIER ;
        }
}

\" {
        BEGIN(CSTRING) ;
}

<CSTRING>\\\"
<CSTRING>[^\"]*
<CSTRING>\"     {
        BEGIN(INITIAL) ;
        const char * str = yytext ;
        uint32_t     len = (uint32_t) strlen(str) ;
        yylval.value = CNAllocateString(str, len, s_valuePool) ;
        return STRING ;
}

. {
        fprintf(stderr, "Unknown character: %c", yytext[0]) ;
        return yytext[0] ;
}
%%

void
CNSetSourceCode(struct CNValueList * strings)
{
        CNInitStringListIterator(&s_string_reader, s_valuePool, strings) ;
}

void
CNDeinitSourceCode(void)
{
        CNDeinitStringListIterator(&s_string_reader) ;
}

static int
getChar(void)
{
        char c = CNGetCharacterFromStringListIterator(&s_string_reader) ;
        printf("getChar: \"%c\"\n", c) ;
        return c ;
}

static bool
isReservedWord(int * rwordid, const char * str)
{
        struct RWord {
                int             rId ;
                const char *    rWord ;
        } ;
        const struct RWord words[] = {
                { .rId = LET,   .rWord = "let"          },
                { .rId = PRINT, .rWord = "print"        },
                { .rId = -1,    .rWord = ""             }  // Terminal word
        } ;
        for(const struct RWord * word = words ; word->rId != -1 ; word++){
                if(strcasecmp(str, word->rWord) == 0){
                        *rwordid = word->rId ;
                        return true ;
                }
        }
        return false ;
}

