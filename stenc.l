%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"
%}

type_int "int"
type_stencil "stencil"

type_qualifier_signed "signed"
type_qualifier_unsigned "unsigned"

operator_assignment "="
operator_plus "+"
operator_minus "-"
operator_multiply "*"
operator_divide "/"
operator_increase "++"
operator_decrease "--"
operator_stencil "$"

control_if "if"
control_else "else"
control_while "while"
control_for "for"

test_inferior "<"
test_superior ">"
test_inferior_equal "<="
test_superior_equal ">="
test_equal "=="
test_not_equal "!="

left_parentheses "("
right_parentheses ")"
left_brace "{"
right_brace "}"
instruction_end ";"
argument_separator ","

display_string "printf"
display_int "printi"

integer [0-9]|([1-9][0-9]*)
integer_array (("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))|("{"((("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))|(((("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))",")*(("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))))"}")

identifier [a-z_][a-zA-Z_]*

%%

{type_int} 			{printf("type_int "); return TYPE_INT;}
{type_stencil} 			{printf("type_stencil "); return TYPE_STENCIL;}

{type_qualifier_signed}		{printf("type_qualifier_signed "); return SIGNED;}
{type_qualifier_unsigned}	{printf("type_qualifier_unsignd "); return UNSIGNED;}

{integer} 			{printf("integer "); yylval.value = atoi(yytext); return INTEGER;}
{integer_array} 		{printf("integer_array "); return STENCIL;}

{operator_assignment} 		{printf("operator_assignment " ); return BINARY_OP_ASSIGNMENT;}
{operator_plus} 		{printf("operator_plus "); return BINARY_OP_ADDITION;}
{operator_minus} 		{printf("operator_minus "); return BINARY_OP_SUBSTRACTION;}
{operator_multiply} 		{printf("operator_multiply "); return BINARY_OP_MULTIPLICATION;}
{operator_divide} 		{printf("operator_divide "); return BINARY_OP_DIVISION;}
{operator_increase} 		{printf("operator_increase "); return UNARY_OP_INCREASE; }
{operator_decrease} 		{printf("operator_decrease "); return UNARY_OP_DECREASE;}
{operator_stencil} 		{printf("operator_stencil "); return BINARY_OP_STENCIL;}

{test_inferior} 		{printf("test_inferior "); return BOOLEAN_LT;}
{test_superior} 		{printf("test_superior "); return BOOLEAN_GT;}
{test_inferior_equal}		{printf("test_inferior_equal "); return BOOLEAN_LE;}
{test_superior_equal} 		{printf("test_superior_equal "); return BOOLEAN_GE;}
{test_equal} 			{printf("test_equal "); return BOOLEAN_EQ;}
{test_not_equal}		{printf("test_not_equal "); return BOOLEAN_NE;}

{left_parentheses} 		{printf("braket_start "); return LEFT_PARENTHESES;}
{right_parentheses} 		{printf("braket_end "); return RIGHT_PARENTHESES;}
{left_brace} 			{printf("block_start "); return LEFT_BRACE;}
{right_brace} 			{printf("block_end "); return RIGHT_BRACE;}
{instruction_end} 		{printf("instruction_end "); return INSTRUCTION_END;}
{argument_separator} 		{printf("argument_separator "); return ARGUMENT_SEPARATOR;}

{control_if} 			{printf("control_if "); return IF;}
{control_else} 			{printf("control_else "); return ELSE;}
{control_while} 		{printf("control_while "); return WHILE;}
{control_for} 			{printf("control_for "); return FOR;}

{display_string} 		{printf("display_string ");}
{display_int} 			{printf("display_integer ");}

{identifier} 			{printf("identifier "); return IDENTIFIER;}

[ \n\t\r] 			{}

. 				{printf("unrecognized character (%c)!\n", yytext[0]);}

%%

/*
int main()
{
	yylex();
	return 0;
}*/
