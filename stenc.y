%{
        #include <stdio.h>
        #include <stdlib.h>
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
        enum relop_enum {EQ, NE, GT, LT, GE, LE} relop_code;
        struct codegen {
                struct symbol *result;
                struct list *code;
                struct list *truelist;
                struct list *falselist;
                struct list *nextlist;
        }gencode;
}

%token <integer_value> INTEGER
%token <string> IDENTIFIER STRING
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGNMENT
%token COMMA SEMICOLON
%token INCREASE DECREASE
%token PRINT_STRING PRINT_INTEGER
%token LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET LEFT_BRACE RIGHT_BRACE
%token BOOL_EQ BOOL_NE BOOL_GT BOOL_LT BOOL_GE BOOL_LE
%token BOOL_AND BOOL_OR BOOL_NOT
%token IF ELSE FOR WHILE

%type <gencode> program
%type <gencode> statement_list
%type <gencode> statement
%type <gencode> control_struct
%type <gencode> if_else_goto
%type <gencode> instruction_block
%type <gencode> expression
%type <gencode> condition
%type <relop_code> relop
%type <gencode> print_function_call

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right ASSIGNMENT
%left BOOL_AND BOOL_OR
%nonassoc BOOL_NOT

%start program

%%

program:
        statement_list '\n'
        {
                printf("program -> statement_list\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
                printf("Match !!!\n");
                return 0;
        }
        ;

statement_list:
        statement_list statement
        {
                printf("statement_list -> statement_list statement\n");
                $$.result = $1.result;
                $$.code = list_concat($1.code, $2.code);
                $$.truelist = list_concat($1.truelist, $2.truelist);
                $$.falselist = list_concat($1.falselist, $2.falselist);
                quad_label($2.code->current_quad);
                list_complete($1.nextlist, $2.code->current_quad->label_name);
                $$.nextlist = NULL;
        }
        | statement
        {
                printf("statement_list -> statement\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                list_complete_to_end($1.nextlist);
                $$.nextlist = NULL;
        }
        ;

statement:
        expression SEMICOLON
	{
		printf("statement -> expression SEMICOLON\n");
		$$.code = $1.code;
		$$.result = NULL;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
	}
        | control_struct
        {
                printf("statement -> control_struct\n");
                $$.code = $1.code;
		$$.result = NULL;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
        }
        | condition
        {
                printf("statement -> condition\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
        }
        ;

control_struct:
        IF LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET instruction_block
        {
                printf("control_struct -> IF (condition)\n");
                // on complète la truelist de condition avec le premier quad du bloc d'instructions
                quad_label($5.code->current_quad);
                list_complete($3.truelist, $5.code->current_quad->label_name);
                // la nextlist est la concaténation de la falselist de condition et et de la nextlist du bloc d'instructions
                $$.nextlist = list_concat($3.falselist, $5.nextlist);
                // génération du goto inconditionnel
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);
                // la nextlist du bloc est la concaténation de la nextlist du bloc et du nouveau quad
                $$.nextlist = list_concat($$.nextlist, list_new(new_quad));
                // le code est le tout
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.code = list_concat($3.code, $5.code);
                $$.result = NULL;
        }
        | IF LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET instruction_block ELSE if_else_goto instruction_block
        {
                printf("control_struct -> IF (condition) else\n");
                // on complète la truelist de condition avec le premier quad du premier bloc d'instructions
                quad_label($5.code->current_quad);
                list_complete($3.truelist, $5.code->current_quad->label_name);
                // on complète la falselist de condition avec le premier quad du second bloc d'instructions
                quad_label($8.code->current_quad);
                list_complete($3.falselist, $8.code->current_quad->label_name);
                // la nextlist est la concaténation des nextlists des deux blocs d'instructions
                $$.nextlist = list_concat($5.nextlist, $8.nextlist);
                // la nextlist est la concaténation de la nextlist et du goto entre if et else
                $$.nextlist = list_concat($$.nextlist, $7.nextlist);
                // on génère un goto inconditionnel vers la sortie
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);
                // la nextlist est la concaténation de la nextlist et du goto nouvellement généré
                $$.nextlist = list_concat($$.nextlist, list_new(new_quad));
                // le code est le tout
                $$.code = list_concat($3.code, list_concat($5.code, $8.code));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.result = NULL;
        }
        ;

if_else_goto:
        {
                // on génère le code d'un goto incomplet entre le if et le else
                $$.result = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);                                                                                                
                $$.code = list_new(new_quad);
                $$.nextlist = list_new(new_quad);
        }
        ;

instruction_block:
        LEFT_BRACE statement_list RIGHT_BRACE
        {
                printf("instruction_block -> { statement_list }\n");
                $$.result = $2.result;
                $$.truelist = $2.truelist;
                $$.falselist = $2.falselist;
                $$.code = $2.code;
                $$.nextlist = $2.nextlist;
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
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
	| IDENTIFIER DECREASE
	{
		printf("expression -> IDENTIFIER DECREASE (low priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
        | expression PLUS expression
        {
                printf("expression -> expression + expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_PLUS, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | expression MINUS expression
        {
                printf("expression -> expression - expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MINUS, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | expression MULTIPLY expression
        {
                printf("expression -> expression * expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MULTIPLY, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | expression DIVIDE expression
        {
                printf("expression -> expression / expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_DIVIDE, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | '(' expression ')'
        {
                printf("expression -> ( expression )\n");
                $$.result = $2.result;
                $$.code = $2.code;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
	| IDENTIFIER ASSIGNMENT expression
	{
		printf("expression -> IDENTIFIER ASSIGNMENT expression\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_ASSIGNMENT, id, $3.result, NULL, false, NULL);
		$$.result = id;
		$$.code = list_concat($3.code, list_new(new_quad));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
	| INCREASE IDENTIFIER
	{
		printf("expression -> INCREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
	| DECREASE IDENTIFIER
	{
		printf("expression -> DECREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
		struct symbol *decr = symbol_new_temp(&symbol_table);
		decr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, decr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
        | INTEGER
        {
                printf("expression -> INTEGER\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->is_constant = true;
                new->int_value = $1;
                $$.result = new;
                $$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | IDENTIFIER
        {
                printf("expression -> IDENTIFIER\n");
                struct symbol *id = symbol_lookup(symbol_table, $1);
                $$.result = id;
                $$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | print_function_call
        ;

condition:
        condition BOOL_OR condition
        {
                printf("condition -> condition BOOL_OR condition\n");
                // on complète la falselist de expression1 par le premier quad de expression2
                list_complete($1.falselist, $3.code->current_quad->label_name);
                // la falselist est la falselist de expression2
                $$.falselist = $3.falselist;
                // la truelist est la concaténation des truelist de expression1 et expression2
                $$.truelist = list_concat($1.truelist, $3.truelist);
                // le code est la concaténation des codes de expression1 et expression2
                $$.code = list_concat($1.code, $3.code);
                $$.result = NULL;
                $$.nextlist = NULL;
        }
        | condition BOOL_AND condition
        {
                printf("condition -> condition BOOL_OR condition\n");
                printf("test %d\n", $3.code->current_quad->id);
                // on complète la truelist de expression1 par le numéro du premier quad de expression2
                list_complete($1.truelist, $3.code->current_quad->label_name);
                // la falselist est la concaténation des falselist de expression1 et expression2
                $$.falselist = list_concat($1.falselist, $3.falselist);
                // la truelist est la truelist de expression2
                $$.truelist = $3.truelist;
                // le code est le code des deux expressions
                $$.code = list_concat($1.code, $3.code);
                $$.result = NULL;
                $$.nextlist = NULL;
        }
        | BOOL_NOT condition
        {
                printf("condition -> BOOL_NOT condition\n");
                // on inverse simplement les truelist et falselist (code et result inchangés)
                $$.result = $2.result;
                $$.code = $2.code;
                $$.truelist = $2.falselist;
                $$.falselist = $2.truelist;
                $$.nextlist = NULL;
        }
        | LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET
        {
                printf("condition -> LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET\n");
                // rien de spécial on transmet juste les listes
                $$.result = $2.result;
                $$.code = $2.code;
                $$.truelist = $2.truelist;
                $$.falselist = $2.falselist;
                $$.nextlist = NULL;
        }
        | IDENTIFIER
        {
                printf("condition -> IDENTIFIER\n");
                // recherche du symbole dans la table
                struct symbol *id = symbol_lookup(symbol_table, $1);
                // génération du goto conditionnel : if ID goto ?
                struct quad *new_quad_true = quad_gen(&quad_list, QUAD_NO_OP, id, NULL, NULL, true, NULL);
                // génération du goto inconditionnel : goto ?
                struct quad *new_quad_false = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);
                // la truelist est le quad contenant le goto conditionnel
                $$.truelist = list_new(new_quad_true);
                // la falselist est le quad contenant le goto inconditionnel
                $$.falselist = list_new(new_quad_false);
                // le code est la concaténation des truelist et falselist
                $$.code = list_concat(list_new(new_quad_true), list_new(new_quad_false));
                $$.result = NULL;
                $$.nextlist = NULL;
        }
        | IDENTIFIER relop IDENTIFIER
        {
                printf("condition -> IDENTIFIER relop IDENTIFIER\n");
                // recherche des deux symboles dans la table
                struct symbol *id1 = symbol_lookup(symbol_table, $1);
                struct symbol *id2 = symbol_lookup(symbol_table, $3);
                // génération du goto conditionnel : if ID1 == ID2 goto ?
                struct quad *new_quad_true;
                switch ($2)
                {
                        case EQ:
                                new_quad_true = quad_gen(&quad_list, QUAD_EQ, id1, id2, NULL, true, NULL);
                                break;
                        case NE:
                                new_quad_true = quad_gen(&quad_list, QUAD_NE, id1, id2, NULL, true, NULL);
                                break;
                        case GT:
                                new_quad_true = quad_gen(&quad_list, QUAD_GT, id1, id2, NULL, true, NULL);
                                break;
                        case LT:
                                new_quad_true = quad_gen(&quad_list, QUAD_LT, id1, id2, NULL, true, NULL);
                                break;
                        case GE:
                                new_quad_true = quad_gen(&quad_list, QUAD_GE, id1, id2, NULL, true, NULL);
                                break;
                        case LE:
                                new_quad_true = quad_gen(&quad_list, QUAD_LE, id1, id2, NULL, true, NULL);
                                break;
                        default:
                                fprintf(stderr, "relop non reconnu...\n");
                                exit(1);
                }
                // génération du goto inconditionnel : goto ?
                struct quad *new_quad_false = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);
                // la truelist est le quad contenant le goto conditionnel
                $$.truelist = list_new(new_quad_true);
                // la falselist est le quad contenant le goto inconditionnel
                $$.falselist = list_new(new_quad_false);
                // le code est la concaténation des truelist et falselist
                $$.code = list_concat(list_new(new_quad_true), list_new(new_quad_false));
                $$.result = NULL;
                $$.nextlist = NULL;
        }
        ;

relop:
        BOOL_EQ
        {
                printf("relop -> BOOL_EQ\n");
                $$ = EQ;
        }
        | BOOL_NE
        {
                printf("relop ->  BOOL_NE\n");
                $$ = NE;
        }
        | BOOL_GT
        {
                printf("relop -> BOOL_GT\n");
                $$ = GT;
        }
        | BOOL_LT
        {
                printf("relop -> BOOL_LT\n");
                $$ = LT;
        }
        | BOOL_GE
        {
                printf("relop -> BOOL_GE\n");
                $$ = GE;
        }
        | BOOL_LE
        {
                printf("relop -> BOOL_LE\n");
                $$ = LE;
        }
        ;

print_function_call:
        PRINT_STRING LEFT_ROUND_BRACKET STRING RIGHT_ROUND_BRACKET
        {
                printf("print_function_call -> PRINT_STRING LEFT_ROUND_BRACKET STRING RIGHT_ROUND_BRACKET\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
		new->string_value = $3;
		new->is_string_litteral = true;
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTF, new, NULL, NULL, false, NULL);
                $$.result = NULL;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET INTEGER RIGHT_ROUND_BRACKET
        {
                printf("print_function_call -> PRINT_INTEGER LEFT_ROUND_BRACKET INTEGER RIGHT_ROUND_BRACKET\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->int_value = $3;
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, new, NULL, NULL, false, NULL);
                $$.result = NULL;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET IDENTIFIER RIGHT_ROUND_BRACKET
        {
                printf("print_function_call -> PRINT_STRING LEFT_ROUND_BRACKET IDENTIFIER RIGHT_ROUND_BRACKET\n");
                struct symbol *id = symbol_lookup(symbol_table, $3);
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, id, NULL, NULL, false, NULL);
                $$.result = NULL;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
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
