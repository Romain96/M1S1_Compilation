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

comment			([/][*][^*]*[*]+([^*/][^*]*[*]+)*[/])|("//".*)
error_comment       	[/][*]
int_array           	"{"[{},0-9\- ]*"}"
integer             	([0-9]|([1-9][0-9]*))
array_reference     	[a-z_][a-zA-Z_0-9]*("["[^ =;]"]")+
identifier          	[a-z_][a-zA-Z_0-9]*
string              	["][^\"]*["]

assignment	    	"="

plus                	"+"
minus               	"-"
multiply            	"*"
divide              	"/"
increase	   	"++"
decrease	   	"--"

bool_eq             	"=="
bool_ne             	"!="
bool_gt             	">"
bool_lt             	"<"
bool_ge             	">="
bool_le             	"<="
bool_and            	"&&"
bool_or             	"||"
bool_not            	"!"

left_round_bracket  	"("
right_round_bracket 	")"
left_brace          	"{"
right_brace         	"}"
left_bracket        	"["
right_bracket       	"]"
semicolon 	    	";"
comma               	","

print               	"print"
if                  	"if"
else                	"else"
for                 	"for"
while               	"while"

type_int            	"int"
type_stencil        	"stencil"
stencil_op          	"$"
stencil             	[a-z_][a-zA-Z_0-9]*"{"([0-9]|([1-9][0-9]*))","([0-9]|([1-9][0-9]*))"}"

main			"main"[ ]*"()"
return 			"return"

%%

{return}[ ]*            { return RETURN; }

{print}f                { return PRINT_STRING; }
{print}i                { return PRINT_INTEGER; }

{main}			{ return MAIN; }

{if}                    { return IF; }
{else}                  { return ELSE; }
{for}                   { return FOR; }
{while}                 { return WHILE; }

{type_int}           	{ return TYPE_INT; }
{type_stencil}{left_bracket}{integer}{comma}{integer}{right_bracket}	{ 
										fprintf(stderr, "Stencils are not yet supported by the compiler\n");
                                                                            	exit(1); 
                                                                        }
{stencil}               {
                            	fprintf(stderr, "Stencil keyword 'stencil' is not yet supported by the compiler\n");
                           	exit(1);
                        }

{integer}               { 
                            	yylval.integer_value = atoi(yytext); 
                            	return INTEGER; 
                        }
{int_array}             {
                            	yylval.array = array_parser_parse_text(yytext);
                            	return INT_ARRAY;
                        }
{array_reference}       {
                            	yylval.array_reference = array_parser_parse_reference(yytext);
                            	char user_identifier_prefix[] = "__mips__";
                            	char *user_id = __array_parser_extract_reference_identifier(yytext);
                            	char *id = malloc((strlen(user_id) + strlen(user_identifier_prefix) + 1) * sizeof(char));
                            	strcat(id, user_identifier_prefix);
                            	strcat(id, user_id);
                            	free(yylval.array_reference->identifier);
                            	yylval.array_reference->identifier = id;
                            	if (symbol_lookup(symbol_table, id) == NULL)
                                	symbol_add(&symbol_table, id);
                            	for (int i = 0; i < yylval.array_reference->number_of_dimensions; i++)
                            	{
                                	if (yylval.array_reference->index_of_dimensions[i].is_identifier)
                                	{
                                    		char *user_index = yylval.array_reference->index_of_dimensions[i].identifier;
                                    		char *index = malloc((strlen(user_index) + strlen(user_identifier_prefix) + 1) * sizeof(char));
                                    		strcat(index, user_identifier_prefix);
                                    		strcat(index, user_index);
                                    		free(yylval.array_reference->index_of_dimensions[i].identifier);
                                    		yylval.array_reference->index_of_dimensions[i].identifier = index;
                                	}
                            	}
                            	return INT_ARRAY_REFERENCE;
                        }
{identifier}          	{
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
{increase}	       	{ return INCREASE; }
{decrease}	     	{ return DECREASE; }

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
{semicolon}	        { return SEMICOLON; }
{left_brace}            { return LEFT_BRACE; }
{right_brace}           { return RIGHT_BRACE; }
{left_bracket}          { return LEFT_BRACKET; }
{right_bracket}         { return RIGHT_BRACKET; }
{left_round_bracket}    { return LEFT_ROUND_BRACKET; }
{right_round_bracket}   { return RIGHT_ROUND_BRACKET; }

{stencil_op}            { 
                  		fprintf(stderr, "Stencil operator '$' is not yet supported by the compiler\n");
                            	exit(1);
                        }

\t|\n|\r                {  }
[ ]                     {  }
{comment}               {  }
{error_comment}         { 
				fprintf(stderr, "lexical error : comment is malformed\n"); 
				exit(1); 
			}
.                       { 
				printf("lexical error : char %s not recognized\n", yytext); 
				exit(1); 
			}

%%
