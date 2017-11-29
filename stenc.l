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

integer             [0-9]|([1-9][0-9]*)
identifier          [a-z_][a-zA-Z_]*
plus                "+"
minus               "-"
multiply            "*"
divide              "/"
assignment	        "="
semicolon 	        ";"
increase	        "++"
decrease	        "--"
print               "print"
left_round_bracket  "("
right_round_bracket ")"
string              ["][^\"]*["]

%%

{print}f                { return PRINT_STRING; }
{print}i                { return PRINT_INTEGER; }

{integer}               { 
                            yylval.integer_value = atoi(yytext); 
                            return INTEGER; 
                        }
{identifier}            { 
                            yylval.string = strdup(yytext);
                            if (symbol_lookup(symbol_table, yytext) == NULL)
                                symbol_add(&symbol_table, yytext); 
                            return IDENTIFIER; 
                        }
{string}                { 
				yylval.string = malloc((strlen(yytext) + 1) * sizeof(char));
				int i;
				for (i = 0; i < strlen(yytext) - 2; i++)
					yylval.string[i] = yytext[i+1];
				yylval.string[i] = '\0';
                            return STRING; 
                        }
{plus}                  { return PLUS; }
{minus}                 { return MINUS; }
{multiply}              { return MULTIPLY; }
{divide}                { return DIVIDE; }
{increase}	            { return INCREASE; }
{decrease}	            { return DECREASE; }
{assignment}	        { return ASSIGNMENT; }
{semicolon}	            { return SEMICOLON; }
{left_round_bracket}    { return LEFT_ROUND_BRACKET; }
{right_round_bracket}   { return RIGHT_ROUND_BRACKET; }
\t|\n|\r                { return yytext[0]; }
[ ]                     {  }
.                       { printf("lexical error : char %s not recognized\n", yytext);}

%%
