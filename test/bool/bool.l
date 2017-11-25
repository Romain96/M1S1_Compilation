%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "symbol_table.h"
    #include "y.tab.h"
    extern struct symbol *symbol_table;
%}

identifier  [a-z_][a-zA-Z_]*
and         "&&"
or          "||"
not         "!"
eq          "=="
ne          "!="
gt          ">"
lt          "<"
ge          ">="
le          "<="

%%

{identifier}    { 
                    yylval.string = strdup(yytext);
                    if (symbol_lookup(symbol_table, yytext) == NULL)
                        symbol_add(&symbol_table, yytext); 
                    return IDENTIFIER; 
                }
{and}           { return AND; }
{or}            { return OR; }
{not}           { return NOT; }
{eq}            { return EQ; }
{ne}            { return NE; }
{gt}            { return GT; }
{lt}            { return LT; }
{ge}            { return GE; }
{le}            { return LE; }
[()]            { return yytext[0]; }
\t|\n|\r        { return yytext[0]; }
[ ]             {  }
.               { printf("lexical error : char %s not recognized\n", yytext);}

%%