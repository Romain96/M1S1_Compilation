%{
	#include <stdio.h>
	#include <stdlib.h>
	extern FILE* yyin;
	int yylex();
	void yyerror(char *);
%}

%union {
	int value;
}

%token FOR WHILE IF ELSE
%token LEFT_BRACE RIGHT_BRACE LEFT_PARENTHESES RIGHT_PARENTHESES LEFT_SQUARED_BRACKET RIGHT_SQUARE_BRACKET
%token INSTRUCTION_END ARGUMENT_SEPARATOR
%token BINARY_OP_ASSIGNMENT BINARY_OP_ADDITION BINARY_OP_SUBSTRACTION BINARY_OP_MULTIPLICATION BINARY_OP_DIVISION BINARY_OP_STENCIL
%token UNARY_OP_INCREASE UNARY_OP_DECREASE
%token BOOLEAN_EQ BOOLEAN_NE BOOLEAN_GE BOOLEAN_LE BOOLEAN_GT BOOLEAN_LT
%token SIGNED UNSIGNED
%token TYPE_INT TYPE_STENCIL
%token INTEGER STENCIL
%token IDENTIFIER

%left '+'
%left '-'
%left '*'
%left '/'

%%

program:
	  instruction_list
		{
			printf("program -> instruction_list\n");
		}
	;

instruction_list:
	  instruction_list instruction
		{
			printf("instruction_list -> instruction_list instruction\n");
		}
	| instruction_list control_block
		{
			printf("instruction_list -> instruction_list control_block\n");
		}
	| instruction
		{
			printf("insruction_list -> instruction\n");
		}
	| control_block
		{
			printf("instruction_list -> control_block\n");
		}
	;

instruction:
	  declaration INSTRUCTION_END
		{
			printf("instruction -> declaration INSTRUCTION_END\n");
		}
	| assignment INSTRUCTION_END
		{
			printf("instruction -> assigmenemt INSTRUCTION_END\n");
		}
	;

control_block:
	  if_else_loop
		{
			printf("control_block -> if_else_loop\n");
		}
	| for_loop
		{
			printf("cnotrol_block -> for_loop\n");
		}
	| while_loop
		{
			printf("control_block -> while_loop\n");
		}
	;


if_else_loop :
	  IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction 
		{
			printf("if_else_loop -> IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction");
		}
	| IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction ELSE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction
		{
			printf("if_else_loop -> IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction instruction ELSE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction\n");
		}
	| IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE
		{
			printf("if_else_loop -> IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE\n");
		}
	| IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE ELSE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE
		{
			printf("if_else_loop -> IF LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE ELSE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE\n");
		}
	;

for_loop:
	  FOR LEFT_PARENTHESES for_loop_init INSTRUCTION_END boolean_condition INSTRUCTION_END for_loop_iterator RIGHT_PARENTHESES instruction
		{
			printf("for_loop -> FOR LEFT_PARENTHESES_for_loop_init INSTRUCTION_END boolean_condition INTRUCTION_END for_loop_iterator RIGHT_PARENTHESES instruction\n");
		}
	| FOR LEFT_PARENTHESES for_loop_init INSTRUCTION_END boolean_condition INSTRUCTION_END for_loop_iterator RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE
		{
			printf("for_loop -> FOR LEFT_PARENTHESES for_loop_init INSTRUCTION_END boolean_condition INSTRUCTION_END for_loop_iterator RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE\n");
		}
	;

while_loop:
	  WHILE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction
		{
			printf("while_loop -> WHILE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES instruction\n");
		}
	| WHILE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE
		{
			printf("while_loop -> WHILE LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES LEFT_BRACE instruction_list RIGHT_BRACE\n");
		}
	;

for_loop_init:
	  type_qualifier type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("for_loop_init -> type_qualifier type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	| type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("for_loop_init -> type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	| IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("for_loop_init -> IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	;

for_loop_iterator:
	  UNARY_OP_INCREASE IDENTIFIER
		{
			printf("for_loop_iterator -> UNARY_OP_INCREASE IDENTIFIER\n");
		}
	| UNARY_OP_DECREASE IDENTIFIER
		{
			printf("for_loop_iterator -> UNARY_OP_DECREASE IDENTIFIER\n");
		}
	| IDENTIFIER UNARY_OP_INCREASE
		{
			printf("for_loop_iterator -> IDENTIFIER UNARY_OP_INCREASE\n");
		}
	| IDENTIFIER UNARY_OP_DECREASE
		{
			printf("for_loop_iterator -> IDENTIFIER UNARY_OP_DECREASE\n");
		}
	;

boolean_condition:
	  boolean_condition boolean_binary_operator boolean_condition
		{
			printf("boolean_condition -> boolean_condition boolean_binary_operator\n");
		}
	| LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES
		{
			printf("boolean_condition -> LEFT_PARENTHESES boolean_condition RIGHT_PARENTHESES\n");
		}
	| true_false_statement
		{
			printf("boolean_condition -> true_false_statement\n");
		}
	;

true_false_statement:
	  IDENTIFIER boolean_binary_operator IDENTIFIER
		{
			printf("true_false_statement -> IDENTIFIER boolean_binary_operator IDENTIFIER\n");
		}
	| IDENTIFIER boolean_binary_operator INTEGER
		{
			printf("true_false_statement -> IDENTIFIER boolean_binary_operator INTEGER\n");
		}
	| INTEGER boolean_binary_operator IDENTIFIER
		{
			printf("true_false_statement -> INTEGER boolean_binary_operator IDENTIFER\n");
		}
	| INTEGER boolean_binary_operator INTEGER
		{
			printf("true_false_statement -> INTEGER boolean_binary_operator INTEGER\n");
		}
	;

boolean_binary_operator:
	  BOOLEAN_EQ
		{
			printf("boolean_binary_operator -> BOOLEAN_EQ\n");
		}
	| BOOLEAN_NE
		{
			printf("boolean_binary_operator -> BOOLEAN_NE\n");
		}
	| BOOLEAN_GT
		{
			printf("boolean_binary_operator -> BOOLEAN_GT\n");
		}
	| BOOLEAN_LT
		{
			printf("boolean_binary_operator -> BOOLEAN_LT\n");
		}
	| BOOLEAN_GE
		{
			printf("boolean_binary_operator -> BOOLEAN_GE\n");
		}
	| BOOLEAN_LE
		{
			printf("boolean_binary_operator -> BOOLEAN_LE\n");
		}
	;

declaration:
	  type_qualifier type_specifier IDENTIFIER
		{
			printf("declaration -> type_qualifier type_specifier IDENTIFIER\n");
		}
	| type_qualifier type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("declaration -> type_qualifier type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	| type_specifier IDENTIFIER
		{
			printf("declaration -> type_specifier IDENTIFIER\n");
		}
	| type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("declaration -> type_specifier IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	;

assignment:
	  IDENTIFIER BINARY_OP_ASSIGNMENT right_value
		{
			printf("assignment -> IDENTIFIER BINARY_OP_ASSIGNMENT right_value\n");
		}
	;

type_qualifier:
	  SIGNED		{printf("type_qualifier -> SIGNED\n");}
	| UNSIGNED		{printf("type_qualifier -> UNSIGNED\n");}
	;

type_specifier:
	  TYPE_INT		{printf("type_specifier -> INT\n");}
	| TYPE_STENCIL		{printf("type_specifier -> STENCIL");}
	;

right_value:
	  addition		{printf("right_value -> addition\n");}
	| substraction		{printf("right_value -> substraction\n");}
	| multiplication	{printf("right_value -> multiplication\n");}
	| division		{printf("right_value -> division\n");}
	| INTEGER		{printf("right_value -> INTEGER\n");}
	;

addition:
	  INTEGER BINARY_OP_ADDITION INTEGER
		{
			printf("addition -> INTEGER BINARY_OP_ADDITION INTEGER\n");
		}
	| INTEGER BINARY_OP_ADDITION IDENTIFIER
		{
			printf("addition -> INTEGER BINARY_OP_ADDITION IDENTIFIER\n");
		}
	| IDENTIFIER BINARY_OP_ADDITION INTEGER
		{
			printf("addition -> IDENTIFIER BINARY_OP_ADDITION INTEGER\n");
		}
	| IDENTIFIER BINARY_OP_ADDITION IDENTIFIER
		{
			printf("addition -> IDENTIFIER BINARY_OP_ADDITION INDENTIFIER\n");
		}
	;

substraction:
	  INTEGER BINARY_OP_SUBSTRACTION INTEGER
		{
			printf("substraction -> INTEGER BINARY_OP_SUBSTRACTION INTEGER\n");
		}
	| INTEGER BINARY_OP_SUBSTRACTION IDENTIFIER
		{
			printf("substraction -> INTEGER BINARY_OP_SUBSTRACTION IDENTIFIER\n");
		}
	| IDENTIFIER BINARY_OP_SUBSTRACTION INTEGER
		{
			printf("substraction -> IDENTIFIER BINARY_OP_SUBSTRACTION INTEGER\n");
		}
	| IDENTIFIER BINARY_OP_SUBSTRACTION IDENTIFIER
		{
			printf("substraction -> IDENTIFIER BINARY_OP_SUBSTRACTION INDENTIFIER\n");
		}
	;

multiplication:
	  INTEGER BINARY_OP_MULTIPLICATION INTEGER
		{
			printf("multiplication -> INTEGER BINARY_OP_MULTIPLICATION INTEGER\n");
		}
	| INTEGER BINARY_OP_MULTIPLICATION IDENTIFIER
		{
			printf("multiplication -> INTEGER BINARY_OP_MULTIPLICATION IDENTIFIER\n");
		}
	| IDENTIFIER BINARY_OP_MULTIPLICATION INTEGER
		{
			printf("multiplication -> IDENTIFIER BINARY_OP_MULTIPLICATION INTEGER\n");
		}
	| IDENTIFIER BINARY_OP_MULTIPLICATION IDENTIFIER
		{
			printf("multiplication -> IDENTIFIER BINARY_OP_MULTIPLICATION INDENTIFIER\n");
		}
	;

division:
	  INTEGER BINARY_OP_DIVISION INTEGER
		{
			printf("division -> INTEGER BINARY_OP_DIVISION INTEGER\n");
		}
	| INTEGER BINARY_OP_DIVISION IDENTIFIER
		{
			printf("division -> INTEGER BINARY_OP_DIVISION IDENTIFIER\n");
		}
	| IDENTIFIER BINARY_OP_DIVISION INTEGER
		{
			printf("division -> IDENTIFIER BINARY_OP_DIVISION INTEGER\n");
		}
	| IDENTIFIER BINARY_OP_DIVISION IDENTIFIER
		{
			printf("division -> IDENTIFIER BINARY_OP_DIVISION INDENTIFIER\n");
		}
	;

%%

int main(int argc, char* argv[])
{	

	if (argc != 2)
	{
		fprintf(stderr, "usage : %s filename\n", argv[0]);
		exit(1);
	}
	yyin = fopen(argv[1], "r");
	return yyparse();

/*


Test de simplification de grammaire




program:
	  instruction_list
		{
			printf("program -> instruction_list\n");
		}
	;

instruction_list:
	  instruction_list instruction_list_BIS
		{
			printf("instruction_list -> instruction_list_BIS instruction\n");
		}
	| instruction_list_BIS
		{
			printf("instruction_list -> instruction_list_BIS control_block\n");
		}
	;

instruction_list_bis:
	  instruction
		{
			printf("insruction_list -> instruction\n");
		}
	| control_block
		{
			printf("instruction_list -> control_block\n");
		}
	;



int_or_id:
	  INTEGER
		{
			printf("int_or_id -> IDENTIFIER boolean_binary_operator INTEGER\n");
		}
	| IDENTIFIER

		{
			printf("int_or_id -> IDENTIFIER boolean_binary_operator IDENTIFIER\n");
		}
	;

true_false_statement:
	  IDENTIFIER boolean_binary_operator int_or_id
		{
			printf("true_false_statement -> IDENTIFIER boolean_binary_operator int_or_id\n");
		}
	| INTEGER boolean_binary_operator int_or_id
		{
			printf("true_false_statement -> INTEGER boolean_binary_operator int_or_id\n");
		}
		;


addition:
	  INTEGER BINARY_OP_ADDITION int_or_id
		{
			printf("addition -> INTEGER BINARY_OP_ADDITION int_or_id\n");
		}
	| IDENTIFIER BINARY_OP_ADDITION int_or_id
		{
			printf("addition -> IDENTIFIER BINARY_OP_ADDITION int_or_id\n");
		}
	;

substraction:
	  INTEGER BINARY_OP_SUBSTRACTION int_or_id
		{
			printf("substraction -> INTEGER BINARY_OP_SUBSTRACTION int_or_id\n");
		}
	| IDENTIFIER BINARY_OP_SUBSTRACTION int_or_id
		{
			printf("substraction -> IDENTIFIER BINARY_OP_SUBSTRACTION int_or_id\n");
		}
	;

multiplication:
	  INTEGER BINARY_OP_MULTIPLICATION int_or_id
		{
			printf("multiplication -> INTEGER BINARY_OP_MULTIPLICATION int_or_id\n");
		}
	| IDENTIFIER BINARY_OP_MULTIPLICATION int_or_id
		{
			printf("multiplication -> IDENTIFIER BINARY_OP_MULTIPLICATION int_or_id\n");
		}
	;

division:
	  INTEGER BINARY_OP_DIVISION int_or_id
		{
			printf("division -> INTEGER BINARY_OP_DIVISION int_or_id\n");
		}
	
	| IDENTIFIER BINARY_OP_DIVISION int_or_id
		{
			printf("division -> IDENTIFIER BINARY_OP_DIVISION int_or_id\n");
		}
	;

*/

}
