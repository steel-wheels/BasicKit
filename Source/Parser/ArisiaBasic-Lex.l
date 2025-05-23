%{
#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNStringValue.h"
#include "CNStringIterator.h"
#include "y.tab.h"

static int
getChar(void) ;

struct CNStringBuffer {
        unsigned int    currentIndex ;
        size_t          buffer_size ;
        char *          buffer ;
} ;

static struct CNValuePool *     s_value_pool ;
static struct CNStringIterator  s_iterator ;
static struct CNStringBuffer    s_buffer ;

void
CNInitLexicalParser(struct CNValuePool * vpool)
{
        s_value_pool = vpool ;
        CNInitStringIterator(&s_iterator, vpool) ;

        struct CNStringBuffer buf = {
                .currentIndex   = 0,
                .buffer_size    = 1024,
                .buffer         = malloc(1024)
        } ;
        s_buffer = buf ;
}

void
CNDeinitLexicalParser(void)
{
        CNDeinitStringIterator(&s_iterator) ;
        free(s_buffer.buffer) ;
}

void
CNSetSourceCodeToLexicalParser(struct CNValueList * statementList)
{
        CNDeinitStringIterator(&s_iterator) ;
        CNSetStringListToStringIterator(&s_iterator, statementList) ;
}

static bool
isReservedWord(int * rwordid, const char * str) ;

static void
appendCharToBuffer(char c)
{
        if(s_buffer.currentIndex >= s_buffer.buffer_size){
                size_t newsize       = s_buffer.buffer_size + 1024 ;
                s_buffer.buffer      = realloc(s_buffer.buffer, newsize) ;
                s_buffer.buffer_size = newsize ;
        }
        s_buffer.buffer[s_buffer.currentIndex] = c ;
        s_buffer.currentIndex += 1 ;
}

static void
appendStringToBuffer(const char * str)
{
        char c ;
        for( ; (c = *str) != '\0' ; str++){
                appendCharToBuffer(c) ;
        }
}

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

%x CSTRING

%%

{SPACE}+
;

{IDENTIFIER}  {
        int rwordid ;
        if(isReservedWord(&rwordid, yytext)){
                yylval->variable = CNMakeVariable(CNNullType, 0) ;
                return rwordid ;
        } else {
                const char * str = yytext ;
                uint32_t len = (uint32_t) strlen(str) ;
                yylval->identifier = CNAllocateStringValue(s_value_pool, len, str) ;
                return IDENTIFIER ;
        }
}

\" {
        BEGIN(CSTRING) ;
        s_buffer.currentIndex = 0 ;
}

<CSTRING>\\\" {
        appendStringToBuffer(yytext) ;
}

<CSTRING>[^\"]* {
        appendStringToBuffer(yytext) ;
}

<CSTRING>\"     {
        BEGIN(INITIAL) ;
        appendCharToBuffer('\0') ;
        const char * buf = s_buffer.buffer ;
        struct CNStringValue * str = CNAllocateStringValue(s_value_pool, (unsigned int) strlen(buf), buf) ;
        yylval->string = str ;
        return STRING ;
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

