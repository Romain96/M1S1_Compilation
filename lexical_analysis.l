%{
	#include <stdio.h>
%}

type_int "int"
type_stencil "stencil"

operator_affectation "="
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

bracket_start "("
bracket_end ")"
block_start "{"
block_end "}"
instruction_end ";"
argument_separator ","

display_string "printf"
display_int "printi"

integer [0-9]|([1-9][0-9]*)
integer_array (("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))|("{"((("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))|(((("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))",")*(("{"[0-9]|([1-9][0-9]*)"}")|("{"(([0-9]|([1-9][0-9]*)),)*([0-9]|([1-9][0-9]*))"}"))))"}")

identifier [a-z_][a-zA-Z_]*

%%

{type_int} printf("type_int ");
{type_stencil} printf("type_stencil ");

{integer} printf("integer ");
{integer_array} printf("integer_array ");

{operator_affectation} printf("operator_affectation ");
{operator_plus} printf("operator_plus ");
{operator_minus} printf("operator_minus ");
{operator_multiply} printf("operator_multiply ");
{operator_divide} printf("operator_divide ");
{operator_increase} printf("operator_increase ");
{operator_decrease} printf("operator_decrease ");
{operator_stencil} printf("operatog_stencil ");

{test_inferior} printf("test_inferior ");
{test_superior} printf("test_superior ");
{test_inferior_equal} printf("test_inferior_equal ");
{test_superior_equal} printf("test_superior_equal ");
{test_equal} printf("test_equal ");

{bracket_start} printf("braket_start ");
{bracket_end} printf("braket_end ");
{block_start} printf("block_start ");
{block_end} printf("block_end ");
{instruction_end} printf("instruction_end ");
{argument_separator} printf("argument_separator ");

{control_if} printf("control_if ");
{control_else} printf("control_else ");
{control_while} printf("control_while ");
{control_for} printf("control_for ");

{display_string} printf("display_string ");
{display_int} printf("display_integer ");

{identifier} printf("identifier ");

[ /n/t] 

%%

int main()
{
	yylex();
	return 0;
}
