%{
    #include <stdio.h>
    #include <string.h>
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
string              ["][^\"]*["]

assignment	        "="

plus                "+"
minus               "-"
multiply            "*"
divide              "/"
increase	        "++"
decrease	        "--"

bool_eq             "=="
bool_ne             "!="
bool_gt             ">"
bool_lt             "<"
bool_ge             ">="
bool_le             "<="
bool_and            "&&"
bool_or             "||"
bool_not            "!"

left_round_bracket  "("
right_round_bracket ")"
left_brace          "{"
right_brace         "}"
semicolon 	        ";"
comma               ","

print               "print"
if                  "if"
else                "else"
for                 "for"
while               "while"

type_int            "int"
type_stencil        "stencil"

%%

{print}f                { return PRINT_STRING; }
{print}i                { return PRINT_INTEGER; }

{if}                    { return IF; }
{else}                  { return ELSE; }
{for}                   { return FOR; }
{while}                 { return WHILE; }

{type_int}              { return TYPE_INT; }
{type_int}("["{integer}"]")*                    { return TYPE_INT_ARRAY; }
{type_stencil}"{"{integer}{comma}{integer}"}"   { return TYPE_STENCIL; }

{integer}               { 
                            yylval.integer_value = atoi(yytext); 
                            return INTEGER; 
                        }
{identifier}            { 
                            char user_identifier_prefix[] = "__mips__";
                            yylval.string = malloc((strlen(yytext) + strlen(user_identifier_prefix) + 1) * sizeof(char));
                            strcat(yylval.string, user_identifier_prefix);
                            strcat(yylval.string, yytext);
                            if (symbol_lookup(symbol_table, yylval.string) == NULL)
                                symbol_add(&symbol_table, yylval.string); 
                            return IDENTIFIER; 
                        }
{string}                { 
				            yylval.string = strdup(yytext);
                            return STRING; 
                        }

{assignment}	        { return ASSIGNMENT; }

{plus}                  { return PLUS; }
{minus}                 { return MINUS; }
{multiply}              { return MULTIPLY; }
{divide}                { return DIVIDE; }
{increase}	            { return INCREASE; }
{decrease}	            { return DECREASE; }

{bool_eq}               { return BOOL_EQ; }
{bool_ne}               { return BOOL_NE; }
{bool_gt}               { return BOOL_GT; }
{bool_lt}               { return BOOL_LT; }
{bool_ge}               { return BOOL_GE; }
{bool_le}               { return BOOL_LE; }
{bool_and}              { return BOOL_AND; }
{bool_or}               { return BOOL_OR; }
{bool_not}              { return BOOL_NOT; }

{comma}                 { return COMMA; }
{semicolon}	            { return SEMICOLON; }
{left_brace}            { return LEFT_BRACE; }
{right_brace}           { return RIGHT_BRACE; }
{left_round_bracket}    { return LEFT_ROUND_BRACKET; }
{right_round_bracket}   { return RIGHT_ROUND_BRACKET; }

\t|\n|\r                { return yytext[0]; }
[ ]                     {  }
.                       { printf("lexical error : char %s not recognized\n", yytext); exit(1); }

%%
