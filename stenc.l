%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "symbol_table.h"
    #include "array_parser.h"
    #include "y.tab.h"
    extern struct symbol *symbol_table;
%}

%option nounput
%option noyywrap

int_array           "{"[{},0-9\- ]*"}"
integer             [-]?([0-9]|([1-9][0-9]*))
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
left_bracket        "["
right_bracket       "]"
semicolon 	        ";"
comma               ","

print               "print"
if                  "if"
else                "else"
for                 "for"
while               "while"

type_int            "int"
type_stencil        "stencil"

main		"main"[ ]*"()"
return 		"return"

%%

{return}[ ]*            { return RETURN; }

{print}f                { return PRINT_STRING; }
{print}i                { return PRINT_INTEGER; }

{main}			        { return MAIN; }

{if}                    { return IF; }
{else}                  { return ELSE; }
{for}                   { return FOR; }
{while}                 { return WHILE; }

{type_int}           	{ return TYPE_INT; }
{type_stencil}{left_bracket}{integer}{comma}{integer}{right_bracket}   { return TYPE_STENCIL; }

{integer}               { 
                            yylval.integer_value = atoi(yytext); 
                            return INTEGER; 
                        }
{int_array}             {
                            yylval.array = array_parser_parse_text(yytext);
                            array_parser_print_struct(yylval.array);
                            printf("int_array is here %s\n", yytext);
                            return INT_ARRAY;
                        }
{identifier}          	{
                            char user_identifier_prefix[] = "__mips__";
                            yylval.string = malloc((strlen(yytext) + strlen(user_identifier_prefix) + 1) * sizeof(char));
                            strcat(yylval.string, user_identifier_prefix);
                            strcat(yylval.string, yytext);
                            if (symbol_lookup(symbol_table, yylval.string) == NULL)
                                symbol_add(&symbol_table, yylval.string); 
                            printf("identifier is fucking here %s (%s)\n", yylval.string, yytext);
                            return IDENTIFIER; 
                        }
{string}                { 
				            yylval.string = strdup(yytext);
                            return STRING; 
                        }

{assignment}	        { printf("assignment is here\n"); return ASSIGNMENT; }

{plus}                  { return PLUS; }
{minus}                 { return MINUS; }
{multiply}              { return MULTIPLY; }
{divide}                { return DIVIDE; }
{increase}	       	    { return INCREASE; }
{decrease}	     	    { return DECREASE; }

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
{left_brace}            { printf("l_br\n"); return LEFT_BRACE; }
{right_brace}           { printf("r_br\n"); return RIGHT_BRACE; }
{left_bracket}          { printf("l_brk\n"); return LEFT_BRACKET; }
{right_bracket}         { printf("r_brk\n"); return RIGHT_BRACKET; }
{left_round_bracket}    { return LEFT_ROUND_BRACKET; }
{right_round_bracket}   { return RIGHT_ROUND_BRACKET; }

\t|\n|\r                {  }
[ ]                     {  }
.                       { printf("lexical error : char %s not recognized\n", yytext); exit(1); }

%%
