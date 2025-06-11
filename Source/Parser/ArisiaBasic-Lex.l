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
isTypeIdentifier(CNValueType * dst, const char * str) ;
static bool
isReservedWord(int * rwordid, const char * str) ;

static uint64_t
stringToInt(const char * src, int base) ;
static uint64_t
binaryToInt(const char * src) ;
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
HEX             [0-9a-fA-F]
BINARY          [0-1]

LESS_THAN       <
LESS_EQUAL      <=
GREATER_THAN    >
GREATE_EQUAL    >=
SHIFT_LEFT      <<
SHIFT_RIGHT     >>

%x CSTRING

%%

{SPACE}+
        ;

{IDENTIFIER}  {
        CNValueType     vtype ;
        int             rwordid ;
        if(isTypeIdentifier(&vtype, yytext)) {
                yylval->valueType = vtype ;
                return TYPE_IDENTIFIER ;
        } else if(isReservedWord(&rwordid, yytext)){
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
        uint64_t val = stringToInt(yytext, 10) ;
        yylval->unsignedIntValue = val ;
        return INT_VALUE ;
}

{NONZERO_DIGIT}{DIGIT}*\.{DIGIT}+ {
        double val = stringToFloat(yytext) ;
        yylval->floatValue = val ;
        return FLOAT_VALUE ;
}

0[xX]{HEX}+ {
        uint64_t val = stringToInt(yytext, 16) ;
        yylval->unsignedIntValue = val ;
        return INT_VALUE ;
}

0[bB]{BINARY}+ {
        uint64_t val = binaryToInt(yytext) ;
        yylval->unsignedIntValue = val ;
        return INT_VALUE ;
}

0 {
        yylval->unsignedIntValue = 0 ;
        return INT_VALUE ;
}

== {
        return OP_EQUAL ;
}

!= {
        return OP_NOT_EQUAL ;
}

{SHIFT_LEFT} {
        return OP_SHIFT_LEFT ;
}

{SHIFT_RIGHT} {
        return OP_SHIFT_RIGHT ;
}

{LESS_EQUAL} {
        return OP_LESS_EQUAL ;
}

{GREATE_EQUAL} {
        return OP_GREATE_EQUAL ;
}

{LESS_THAN} {
        return OP_LESS_THAN ;
}

{GREATER_THAN} {
        return OP_GREATER_THAN ;
}

\| {
        return OP_BIT_OR ;
}

& {
        return OP_BIT_AND ;
}

\^ {
        return OP_BIT_XOR ;
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
isTypeIdentifier(CNValueType * dst, const char * str)
{
        static const CNValueType s_types[] = {
                CNBooleanType,
                CNUnsignedIntType,
                CNSignedIntType,
                CNFloatType,
                CNStringType,
                //CNArrayType,
                //CNDictionaryType,
                //CNCodeType
                CNNullType
        } ;
        for(unsigned int i=0 ; s_types[i] != CNNullType ; i++){
                const char * name = CNValueTypeName(s_types[i]) ;
                if(strcasecmp(name, str) == 0){
                        *dst = s_types[i] ;
                        return true ;
                }
        }
        return false ;
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
                { .rId = OP_DIV,        .rWord = "div"         },
                { .rId = ELSE,          .rWord = "else"        },
                { .rId = ENDIF,         .rWord = "endif"       },
                { .rId = IF,            .rWord = "if"          },
                { .rId = LET,           .rWord = "let"         },
                { .rId = OP_LOG_NOT,    .rWord = "not"         },
                { .rId = OP_MOD,        .rWord = "mod"         },
                { .rId = OP_OR,         .rWord = "or"          },
                { .rId = PRINT,         .rWord = "print"       },
                { .rId = FALSE_VALUE,   .rWord = "false"       },
                { .rId = THEN,          .rWord = "then"        },
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
stringToInt(const char * src, int base)
{
        char * endptr ;
        uint64_t val = strtoull(yytext, &endptr, base) ;
        if(endptr != NULL){
                if(*endptr == '\0'){
                        return val ;
                }
        }
        CNInterface()->printf("[Error] Failed to convert to int: \"%s\"\n", src) ;
        return 0 ;
}

static uint64_t
binaryToInt(const char * src)
{
        uint64_t result = 0 ;
        const char * ptr = src + 2 ; // skip "0b"
        char c ;
        for( ; (c = *ptr) != '\0' ; ptr++){
                result = (result << 1) | (c - '0') ;
        }
        return result ;
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
