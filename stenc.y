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
	| instruction
		{
			printf("insruction_list -> instruction\n");
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
}
