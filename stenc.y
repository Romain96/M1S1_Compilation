%{
        #include <stdio.h>
        #include <stdbool.h>
        #include "symbol_table.h"
        #include "quad.h"
        #include "list.h"
        #include "mips_generator.h"
        void yyerror(char*);
        int yylex();

        // la table des symboles
        struct symbol *symbol_table = NULL;
        struct quad *quad_list = NULL;
%}

%union
{
        int integer_value;
        char *string;
        struct codegen {
                struct symbol *result;
                struct list *code;
        }gencode;
}

%token <integer_value> INTEGER
%token <string> IDENTIFIER STRING
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGNMENT
%token SEMICOLON
%token INCREASE DECREASE
%token PRINT_STRING PRINT_INTEGER
%token LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET

%type <gencode> line
%type <gencode> expression
%type <gencode> expression_list
%type <gencode> print_function_call

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right ASSIGNMENT

%start line

%%

line:
        expression_list '\n'
        {
                printf("line -> expression_list\n");
                $$.result = $1.result;
                $$.code = $1.code;
                printf("Match !!!\n");
                return 0;
        }
        ;

expression_list:
	expression_list expression SEMICOLON
	{
		printf("expression_list -> expression_list expression SEMICOLON\n");
		$$.code = list_concat($1.code, $2.code);	
		$$.result = NULL;
	}
	| expression SEMICOLON
	{
		printf("expression_list -> expression SEMICOLON\n");
		$$.code = $1.code;
		$$.result = NULL;
	}
	;

expression:
	IDENTIFIER INCREASE
	{
		printf("expression -> IDENTIFIER INCREASE (low priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, -1);
		$$.result = res;
		$$.code = list_new(new_quad);
	}
	| IDENTIFIER DECREASE
	{
		printf("expression -> IDENTIFIER DECREASE (low priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, incr, res, false, -1);
		$$.result = res;
		$$.code = list_new(new_quad);
	}
        | expression PLUS expression
        {
                printf("expression -> expression + expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_PLUS, $1.result, $3.result, res, false, -1);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression MINUS expression
        {
                printf("expression -> expression - expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MINUS, $1.result, $3.result, res, false, -1);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression MULTIPLY expression
        {
                printf("expression -> expression * expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MULTIPLY, $1.result, $3.result, res, false, -1);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression DIVIDE expression
        {
                printf("expression -> expression / expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_DIVIDE, $1.result, $3.result, res, false, -1);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | '(' expression ')'
        {
                printf("expression -> ( expression )\n");
                $$.result = $2.result;
                $$.code = $2.code;
        }
	| IDENTIFIER ASSIGNMENT expression
	{
		printf("expression -> IDENTIFIER ASSIGNMENT expression\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_ASSIGNMENT, id, $3.result, NULL, false, -1);
		$$.result = id;
		$$.code = list_concat($3.code, list_new(new_quad));
	}
	| INCREASE IDENTIFIER
	{
		printf("expression -> INCREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, -1);
		$$.result = res;
		$$.code = list_new(new_quad);
	}
	| DECREASE IDENTIFIER
	{
		printf("expression -> DECREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
		struct symbol *decr = symbol_new_temp(&symbol_table);
		decr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, decr, res, false, -1);
		$$.result = res;
		$$.code = list_new(new_quad);
	}
        | INTEGER
        {
                printf("expression -> INTEGER\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->is_constant = true;
                new->int_value = $1;
                $$.result = new;
                $$.code = NULL;
                
        }
        | IDENTIFIER
        {
                printf("expression -> IDENTIFIER\n");
                struct symbol *id = symbol_lookup(symbol_table, $1);
                $$.result = id;
                $$.code = NULL;
        }
        | print_function_call
        ;

print_function_call:
        PRINT_STRING LEFT_ROUND_BRACKET STRING RIGHT_ROUND_BRACKET
        {
                struct symbol *new = symbol_new_temp(&symbol_table);
		new->string_value = $3;
		new->is_string_litteral = true;
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTF, new, NULL, NULL, false, -1);
                $$.result = NULL;
                $$.code = list_new(new_quad);
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET INTEGER RIGHT_ROUND_BRACKET
        {
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->int_value = $3;
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, new, NULL, NULL, false, -1);
                $$.result = NULL;
                $$.code = list_new(new_quad);
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET IDENTIFIER RIGHT_ROUND_BRACKET
        {
                struct symbol *id = symbol_lookup(symbol_table, $3);
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, id, NULL, NULL, false, -1);
                $$.result = NULL;
                $$.code = list_new(new_quad);
        }
        ;

%%

int main()
{
        printf("Enter an arithmetic expression\n");
        yyparse();
        symbol_print(symbol_table);
        quad_print(quad_list);

        // !! experimental !!
        struct mips_generator *mips = mips_setup("output.s", symbol_table, quad_list);
        mips_generate_assembly_code(mips);

        return 0;
}
