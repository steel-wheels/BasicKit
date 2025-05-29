%{
#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNStringValue.h"
#include "CNStringIterator.h"
#include "y.tab.h"

static int
getChar(void) ;

struct CNStringBuffer {
        index_t         currentIndex ;
        size_t          buffer_size ;
        char *          buffer ;
} ;

static unsigned int             s_current_line ;
static struct CNValuePool *     s_value_pool ;
static struct CNStringIterator  s_iterator ;
static struct CNStringBuffer    s_buffer ;

void
CNInitLexicalParser(struct CNValuePool * vpool)
{
        s_value_pool   = vpool ;
        s_current_line = 1 ;
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

unsigned int
CNGetCurrentParsingLine(void)
{
        return s_current_line ;
}

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

static bool
isReservedWord(int * rwordid, const char * str) ;

static uint64_t
stringToInt(const char * src) ;
static double
stringToFloat(const char * src) ;

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
DIGIT           [0-9]
NONZERO_DIGIT   [1-9]

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

{NONZERO_DIGIT}{DIGIT}* {
        uint64_t val = stringToInt(yytext) ;
        yylval->unsignedIntValue = val ;
        return INT_VALUE ;
}

{NONZERO_DIGIT}{DIGIT}*\.{DIGIT}+ {
        double val = stringToFloat(yytext) ;
        yylval->floatValue = val ;
        return FLOAT_VALUE ;
}

\n {
        s_current_line += 1 ;
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

. {
        return yytext[0] ;
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
                { .rId = OP_AND,        .rWord = "and"         },
                { .rId = LET,           .rWord = "let"         },
                { .rId = OP_OR,         .rWord = "or"          },
                { .rId = PRINT,         .rWord = "print"       },
                { .rId = FALSE_VALUE,   .rWord = "false"       },
                { .rId = TRUE_VALUE,    .rWord = "true"        },
                { .rId = -1,            .rWord = ""            }  // Terminal word
        } ;
        for(const struct RWord * word = words ; word->rId != -1 ; word++){
                if(strcasecmp(str, word->rWord) == 0){
                        *rwordid = word->rId ;
                        return true ;
                }
        }
        return false ;
}

static uint64_t
stringToInt(const char * src)
{
        char * endptr ;
        uint64_t val = strtoull(yytext, &endptr, 10) ;
        if(endptr != NULL){
                if(*endptr == '\0'){
                        return val ;
                }
        }
        CNInterface()->printf("[Error] Failed to convert to int: \"%s\"\n", src) ;
        return 0 ;
}

static double
stringToFloat(const char * src)
{
        char * endptr ;
        double val = strtod(yytext, &endptr) ;
        if(endptr != NULL){
                if(*endptr == '\0'){
                        return val ;
                }
        }
        CNInterface()->printf("[Error] Failed to convert to float: \"%s\"\n", src) ;
        return 0.0 ;
}
