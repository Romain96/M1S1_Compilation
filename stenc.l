%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "symbol_table.h"
    #include "y.tab.h"
    extern struct symbol *symbol_table;
%}

%option nounput
%option noyywrap

integer         [0-9]|([1-9][0-9]*)
identifier      [a-z_][a-zA-Z_]*
plus            "+"
minus           "-"
multiply        "*"
divide          "/"
assignment	"="

%%

{integer}       { 
                    yylval.integer_value = atoi(yytext); 
                    return INTEGER; 
                    }
{identifier}    { 
                    yylval.string = strdup(yytext);
                    if (symbol_lookup(symbol_table, yytext) == NULL)
                        symbol_add(&symbol_table, yytext); 
                    return IDENTIFIER; 
                }
{plus}          { return PLUS; }
{minus}         { return MINUS; }
{multiply}      { return MULTIPLY; }
{divide}        { return DIVIDE; }
{assignment}	{ return ASSIGNMENT; }
[()]            { return yytext[0]; }
\t|\n|\r        { return yytext[0]; }
[ ]             {  }
.               { printf("lexical error : char %s not recognized\n", yytext);}

%%
