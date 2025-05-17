%{
#include "ArisiaBasic.h"
#include "y.tab.h"

static int
getChar(void) ;

#define YY_INPUT(buf,result,max_size) \
    { \
        int c = getChar(); \
        result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

%}

%option bison-bridge bison-locations
%option noyywrap

SPACE  [ \t]

%%

{SPACE}+
        ;

%%

static int
getChar(void)
{
        return EOF ;
}
