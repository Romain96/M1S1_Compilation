%{
        #include <stdio.h>
        #include <stdbool.h>
        #include "symbol_table.h"
        #include "quad.h"
        #include "list.h"
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
%token <string> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE

%type <gencode> expression
%type <gencode> line

%left PLUS MINUS
%left MULTIPLY DIVIDE

%start line

%%

line:
        expression '\n'
        {
                printf("line -> expression\n");
                $$.result = $1.result;
                $$.code = $1.code;
                printf("Match !!!\n");
                return 0;
        }
        ;

expression:
        expression PLUS expression
        {
                printf("expression -> expression + expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_PLUS, $1.result, $3.result, res);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression MINUS expression
        {
                printf("expression -> expression - expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MINUS, $1.result, $3.result, res);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression MULTIPLY expression
        {
                printf("expression -> expression * expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MULTIPLY, $1.result, $3.result, res);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | expression DIVIDE expression
        {
                printf("expression -> expression / expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_DIVIDE, $1.result, $3.result, res);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
        }
        | '(' expression ')'
        {
                printf("expression -> ( expression )\n");
                $$.result = $2.result;
                $$.code = $2.code;
        }
        | INTEGER
        {
                printf("expression -> INTEGER\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->is_constant = true;
                new->value = $1;
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
        ;

%%

int main()
{
        printf("Enter an arithmetic expression\n");
        yyparse();
        symbol_print(symbol_table);
        quad_print(quad_list);
        return 0;
}