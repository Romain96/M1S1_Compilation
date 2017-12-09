%{
        #include <stdio.h>
        #include <stdlib.h>
        #include <stdbool.h>
        #include <ctype.h>
        #include <unistd.h>
        #include "symbol_table.h"
        #include "quad.h"
        #include "list.h"
        #include "mips_generator.h"
        #include "array_parser.h"
        #define MIPS_REGISTER_SIZE_IN_BYTES 4
        void yyerror(char*);
        int yylex();
        FILE *yyin;

        // la table des symboles
        struct symbol *symbol_table = NULL;
        // liste de quads (le code généré)
        struct quad *quad_list = NULL;

        // vraiables globales liées aux options du programme
        bool _verbose_output = false;   // affiche les règles de la grammaire à chaque action
        bool _print_symbol_table = false;
        bool _print_quad_list = false;
        bool _mips_verbose = false;     // affiche les actions faites par le mips_generator
        char * _input_c_file = NULL;
        char * _output_s_file = NULL;
%}

%error-verbose

%union
{
        int integer_value;
        char *string;
        struct stenc_array *array;
        struct stenc_array *array_reference;
        enum relop_enum {EQ, NE, GT, LT, GE, LE} relop_code;
        enum for_iterator_enum {INCR, DECR} for_iterator_code;
        struct codegen {
                struct symbol *result;
                struct list *code;
                struct list *truelist;
                struct list *falselist;
                struct list *nextlist;
                struct stenc_array *array_value;
        }gencode;
}

%token <integer_value> INTEGER
%token <string> IDENTIFIER STRING
%token <array> INT_ARRAY
%token <array_reference> INT_ARRAY_REFERENCE
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGNMENT
%token COMMA SEMICOLON
%token INCREASE DECREASE
%token PRINT_STRING PRINT_INTEGER
%token LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET
%token BOOL_EQ BOOL_NE BOOL_GT BOOL_LT BOOL_GE BOOL_LE
%token BOOL_AND BOOL_OR BOOL_NOT
%token IF ELSE FOR WHILE
%token TYPE_INT TYPE_INT_ARRAY TYPE_STENCIL
%token RETURN MAIN

%type <gencode> program
%type <gencode> main_func
%type <gencode> statement_list
%type <gencode> statement
%type <gencode> control_struct
%type <gencode> if_else_goto for_init
%type <for_iterator_code> for_iterator
%type <gencode> instruction_block
%type <gencode> expression
%type <gencode> declaration_or_assignment
%type <gencode> condition
%type <relop_code> relop
%type <gencode> print_function_call

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right ASSIGNMENT
%left BOOL_AND BOOL_OR
%nonassoc BOOL_NOT
%nonassoc IF ELSE

%start program

%%

program:
        main_func
        {
                if (_verbose_output)
                        printf("program -> main_func\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
                return 0;
        }
        ;

main_func:
	TYPE_INT MAIN LEFT_BRACE statement_list RETURN statement RIGHT_BRACE
	{
                if (_verbose_output)
		        printf("main_func -> TYPE_INT { statement_list RETURN INTEGER ; }\n");
		$$.result = $6.result;
		$$.code = list_concat($4.code, $6.code);
		$$.truelist = NULL;
		$$.falselist = NULL;
		$$.nextlist = NULL;
                $$.array_value = NULL;
	}
	| TYPE_INT MAIN LEFT_BRACE RETURN statement RIGHT_BRACE
	{
                if (_verbose_output)
		        printf("main_func -> TYPE_INT { RETURN INTEGER ; }\n");
		$$.result = $5.result;
		$$.code = $5.code;
		$$.truelist = NULL;
		$$.falselist = NULL;
		$$.nextlist = NULL;
                $$.array_value = NULL;
	}
	;	

statement_list:
        statement_list statement
        {
                if (_verbose_output)
                        printf("statement_list -> statement_list statement\n");
                $$.result = $1.result;
                $$.code = list_concat($1.code, $2.code);
                $$.truelist = NULL;
                $$.falselist = NULL;
                if ($1.code != NULL && $2.code != NULL)
                {
                        quad_label($2.code->current_quad);
                        list_complete($1.nextlist, $2.code->current_quad->label_name);
                        list_complete_to_end($2.nextlist);
                }
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | statement
        {
                if (_verbose_output)
                        printf("statement_list -> statement\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = $1.truelist;
                $$.falselist = $1.truelist;
                list_complete_to_end($1.nextlist);
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        ;

statement:
        expression SEMICOLON
	{
                if (_verbose_output)
		        printf("statement -> expression SEMICOLON\n");
		$$.code = $1.code;
		$$.result = $1.result;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
                $$.array_value = NULL;
	}
        | control_struct
        {
                if (_verbose_output)
                        printf("statement -> control_struct\n");
                $$.code = $1.code;
		$$.result = NULL;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                $$.nextlist = $1.nextlist;
                $$.array_value = NULL;
        }
        ;

control_struct:
        IF LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET instruction_block
        {
                if (_verbose_output)
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
                $$.array_value = NULL;
        }
        | IF LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET instruction_block ELSE if_else_goto instruction_block
        {
                if (_verbose_output)
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
                $$.array_value = NULL;
        }
        | WHILE LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET instruction_block
        {
                if (_verbose_output)
                        printf("control_struct -> WHILE (condition)\n");
                // on complète la truelist de condition par le premier quad du bloc d'instruction
                quad_label($5.code->current_quad);
                list_complete($3.truelist, $5.code->current_quad->label_name);
                // la nextlist est la falselist de la condition
                $$.nextlist = $3.falselist;
                // génération du goto inconditionnel
                quad_label($3.code->current_quad);
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, $3.code->current_quad->label_name);
                // on complète la nextlist du bloc d'instructions avec le premier quad de la condition
                list_complete($5.nextlist, $3.code->current_quad->label_name);
                // le code est le tout
                $$.result = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.code = list_concat($3.code, list_concat(list_new(new_quad), $5.code));
                $$.array_value = NULL;
        }
        | FOR LEFT_ROUND_BRACKET for_init SEMICOLON condition SEMICOLON for_iterator RIGHT_ROUND_BRACKET instruction_block
        {
                if (_verbose_output)
                        printf("control_struct -> FOR (for_init; condition; for_iterator)\n");
                // on complète la truelist de condition par le premier quad du bloc d'instructions
                quad_label($9.code->current_quad);
                list_complete($5.truelist, $9.code->current_quad->label_name);
                // la nextlist est la falselist de condition
                $$.nextlist = $5.falselist;
                // on génère l'incrémentation/décrémentation
                struct quad *iterator;
                struct symbol *incr = symbol_new_temp(&symbol_table);
                struct symbol *res = symbol_new_temp(&symbol_table);
                incr->int_value = 1;
                switch ($7)
                {
                        case INCR:	                
	                        
		                iterator = quad_gen(&quad_list, QUAD_PLUS, $3.result, incr, res, false, NULL);
                                break;
                        case DECR:
                                iterator = quad_gen(&quad_list, QUAD_MINUS, $3.result, incr, res, false, NULL);
                                break;
                        default:
                                fprintf(stderr, "ni incr ni decr ?\n");
                                exit(1);
                }
                // et le goto inconditionnel vers la condition
                quad_label($5.code->current_quad);
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, $5.code->current_quad->label_name);
                // le code est le tout
                $$.code = list_concat($3.code, list_concat($5.code, list_concat($9.code, list_concat(list_new(iterator), list_new(new_quad)))));
                $$.result = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.array_value = NULL;
        }
        ;

if_else_goto:
        {
                if (_verbose_output)
                        printf("if_else_goto -> epsilon");
                // on génère le code d'un goto incomplet entre le if et le else
                $$.result = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                struct quad *new_quad = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, NULL);                                                                                                
                $$.code = list_new(new_quad);
                $$.nextlist = list_new(new_quad);
                $$.array_value = NULL;
        }
        ;

for_init:
        IDENTIFIER ASSIGNMENT expression
	{
                if (_verbose_output)
		        printf("for_init -> IDENTIFIER ASSIGNMENT expression\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
                // l'id doit être déclaré
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                // l'id est set (qu'il l'ai été ou non)
                id->is_set = true;
		struct quad *new_quad = quad_gen(&quad_list, QUAD_ASSIGNMENT, id, $3.result, NULL, false, NULL);
		$$.result = id;
		$$.code = list_concat($3.code, list_new(new_quad));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
	}
        ;

for_iterator:
        IDENTIFIER INCREASE
        {
                $$ = INCR;
        }
        | IDENTIFIER DECREASE
        {
                $$ = DECR;
        }
        ;

instruction_block:
        LEFT_BRACE statement_list RIGHT_BRACE
        {
                if (_verbose_output)
                        printf("instruction_block -> { statement_list }\n");
                $$.result = $2.result;
                $$.truelist = $2.truelist;
                $$.falselist = $2.falselist;
                $$.code = $2.code;
                $$.nextlist = $2.nextlist;
                $$.array_value = $2.array_value;
        }
        ;

expression:
        IDENTIFIER INCREASE
	{
                if (_verbose_output)
		        printf("expression -> IDENTIFIER INCREASE (low priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
                // l'id doit être déclaré et initialisé
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                if (!id->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id->identifier+8);
                        exit(1);
                }

		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
	}
	| IDENTIFIER DECREASE
	{
                if (_verbose_output)
		        printf("expression -> IDENTIFIER DECREASE (low priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
                // l'id doit être déclaré et initialisé
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                if (!id->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id->identifier+8);
                        exit(1);
                }

		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
	}
        | expression PLUS expression
        {
                if (_verbose_output)
                        printf("expression -> expression + expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_PLUS, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | expression MINUS expression
        {
                if (_verbose_output)
                        printf("expression -> expression - expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MINUS, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | expression MULTIPLY expression
        {
                if (_verbose_output)
                        printf("expression -> expression * expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_MULTIPLY, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | expression DIVIDE expression
        {
                if (_verbose_output)
                        printf("expression -> expression / expression\n");
                struct symbol *res = symbol_new_temp(&symbol_table);
                struct quad *new = quad_gen(&quad_list, QUAD_DIVIDE, $1.result, $3.result, res, false, NULL);
                $$.result = res;
                $$.code = list_concat(list_concat($1.code, $3.code), list_new(new));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | '(' expression ')'
        {
                if (_verbose_output)
                        printf("expression -> ( expression )\n");
                $$.result = $2.result;
                $$.code = $2.code;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | declaration_or_assignment
        {
                if (_verbose_output)
                        printf("expression -> declaration_or_assignment\n");
                $$.result = $1.result;
                $$.code = $1.code;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = $1.array_value;
        }
	| INCREASE IDENTIFIER
	{
                if (_verbose_output)
		        printf("expression -> INCREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
                // l'id doit être déclaré et initialisé
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                if (!id->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id->identifier+8);
                        exit(1);
                }

		struct symbol *incr = symbol_new_temp(&symbol_table);
		incr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_PLUS, id, incr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
	}
	| DECREASE IDENTIFIER
	{
                if (_verbose_output)
		        printf("expression -> DECREASE IDENTIFIER (high priority)\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
                // l'id doit être déclaré et initialisé
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                if (!id->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id->identifier+8);
                        exit(1);
                }

		struct symbol *decr = symbol_new_temp(&symbol_table);
		decr->int_value = 1;
		struct symbol *res = symbol_new_temp(&symbol_table);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_MINUS, id, decr, res, false, NULL);
		$$.result = res;
		$$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
	}
        | INTEGER
        {
                if (_verbose_output)
                        printf("expression -> INTEGER\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->is_constant = true;
                new->int_value = $1;
                $$.result = new;
                $$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | INT_ARRAY
        {
                if (_verbose_output)
                        printf("expression -> INT_ARRAY\n");
                $$.result = NULL;
                $$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = $1;
        }
        | IDENTIFIER
        {
                if (_verbose_output)
                        printf("expression -> IDENTIFIER\n");
                struct symbol *id = symbol_lookup(symbol_table, $1);
                $$.result = id;
                $$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | print_function_call
        ;

declaration_or_assignment: 
        TYPE_INT IDENTIFIER
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> TYPE_INT IDENTIFIER\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
                // maintenant l'id est déclaré mais non initialisé
                id->is_declared = true;
		$$.result = id;
		$$.code = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
        }
	| IDENTIFIER ASSIGNMENT expression
	{
                if (_verbose_output)
		        printf("declaration_or_assignment -> IDENTIFIER ASSIGNMENT expression\n");
		struct symbol *id = symbol_lookup(symbol_table, $1);
                // l'id doit avoir été déclaré précédemment
                if (!id->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id->identifier+8);
                        exit(1);
                }
                // l'id est désormais initialisé
                id->is_set = true;
		struct quad *new_quad = quad_gen(&quad_list, QUAD_ASSIGNMENT, id, $3.result, NULL, false, NULL);
		$$.result = id;
		$$.code = list_concat($3.code, list_new(new_quad));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
	}
        | IDENTIFIER ASSIGNMENT INT_ARRAY_REFERENCE
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> IDENTIFIER ASSIGNMENT INT_ARRAY_REFERENCE\n");
                // récupération du symbole de la variable à affecter
                struct symbol *id = symbol_lookup(symbol_table, $1);
                // la variable doit être de type int (pas int_array ou string_litteral)
                if (id->is_string_litteral)
                {
                        fprintf(stderr, "semantic error : cannot assign a integer (array reference) to the string litteral variable %s\n",
                        id->string_value);
                        exit(1);                
                }
                if (id->is_int_array)
                {
                        fprintf(stderr, "semantic error : cannot assign a integer (array reference) to the integer array variable %s\n",
                        id->string_value);
                        exit(1);         
                }

                // récupération du symbole contenant le nom du tableau
                struct symbol *arr = symbol_lookup(symbol_table, $3->identifier);
                // maintenant l'id est affecté (qu'il l'est été ou non)
                id->is_set = true;                // vérification de la validité des indices
                for (int i = 0; i < arr->int_array_value->number_of_dimensions; i++)
                {
                        // chaque indice doit être inférieur à la taile de la dimension correspondante
                        if ($3->index_of_dimensions[i] >= arr->int_array_value->size_of_dimensions[i])
                        {
                                fprintf(stderr, "semantic error : cannot access index %d of array %s which exceeds size the size of its dimension which is %d\n", 
                                $3->index_of_dimensions[i], arr->identifier + 8, arr->int_array_value->size_of_dimensions[i]);
                                exit(1);
                        }
                }

                // calcul de l'adresse de l'élément à accéder
                array_parser_print($3);
                array_parser_print(arr->int_array_value);
                int address = 0;
                int i;
                for (i = 0; i < arr->int_array_value->number_of_dimensions - 1; i++)
                {
                        printf ("i %d\n", i);
                        address += $3->index_of_dimensions[i] * arr->int_array_value->size_of_dimensions[i + 1];
                        printf("index %d, size %d\n", $3->index_of_dimensions[i], arr->int_array_value->size_of_dimensions[i + 1]);
                        printf("address loop %d\n", address);
                }
                address += $3->index_of_dimensions[i];
                printf("address before MIPS_REGISTER %d\n", address);
                address = address * MIPS_REGISTER_SIZE_IN_BYTES;
                printf("and after %d\n", address);
                // ajout d'un nouveau symbole contenant l'adresse
                struct symbol *addr = symbol_new_temp(&symbol_table);
                addr->int_value = address;
                // génération du quad (QUAD_ARRAY_READ)
                struct quad *new_quad = quad_gen(&quad_list, QUAD_ARRAY_READ, id, arr, addr, false, NULL);
                $$.result = id;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | INT_ARRAY_REFERENCE ASSIGNMENT IDENTIFIER
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> INT_ARRAY_REFERENCE ASSIGNMENT IDENTIFIER\n");
                // récupération du symbole de la variable à affecter
                struct symbol *id = symbol_lookup(symbol_table, $3);
                // la variable doit être de type int (pas int_array ou string_litteral)
                if (id->is_string_litteral)
                {
                        fprintf(stderr, "semantic error : cannot assign a integer (array reference) to the string litteral variable %s\n",
                        id->string_value);
                        exit(1);                
                }
                if (id->is_int_array)
                {
                        fprintf(stderr, "semantic error : cannot assign a integer (array reference) to the integer array variable %s\n",
                        id->string_value);
                        exit(1);         
                }

                // récupération du symbole contenant le nom du tableau
                struct symbol *arr = symbol_lookup(symbol_table, $1->identifier);
                // maintenant le tableau est affecté (qu'il l'est été ou non)
                arr->is_set = true;
                // vérification de la validité des indices
                for (int i = 0; i < arr->int_array_value->number_of_dimensions; i++)
                {
                        // chaque indice doit être inférieur à la taile de la dimension correspondante
                        if ($1->index_of_dimensions[i] >= arr->int_array_value->size_of_dimensions[i])
                        {
                                fprintf(stderr, "semantic error : cannot access index %d of array %s which exceeds size the size of its dimension which is %d\n", 
                                $1->index_of_dimensions[i], arr->identifier + 8, arr->int_array_value->size_of_dimensions[i]);
                                exit(1);
                        }
                }

                // calcul de l'adresse de l'élément à accéder
                int address = 0;
                int i = 0;
                for (i = 0; i < arr->int_array_value->number_of_dimensions - 1; i++)
                {
                        address += arr->int_array_value->size_of_dimensions[i] * $1->index_of_dimensions[i];
                }
                address += $1->index_of_dimensions[i];
                address = address * MIPS_REGISTER_SIZE_IN_BYTES;
                // ajout d'un nouveau symbole contenant l'addresse
                struct symbol *addr = symbol_new_temp(&symbol_table);
                addr->int_value = address;
                // génération du quad (QUAD_ARRAY_WRITE)
                struct quad *new_quad = quad_gen(&quad_list, QUAD_ARRAY_WRITE, arr, id, addr, false, NULL);
                $$.result = id;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | INT_ARRAY_REFERENCE ASSIGNMENT INTEGER
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> INT_ARRAY_REFERENCE ASSIGNMENT INTEGER\n");
                // création d'un nouveau temporaire contenant la valeur à affecter
                struct symbol *id = symbol_new_temp(&symbol_table);
                id->int_value = $3;
                // récupération du symbole contenant le nom du tableau
                struct symbol *arr = symbol_lookup(symbol_table, $1->identifier);
                // maintenant le tableau est affecté (qu'il l'est été ou non)
                arr->is_set = true;
                // vérification de la validité des indices
                for (int i = 0; i < arr->int_array_value->number_of_dimensions; i++)
                {
                        // chaque indice doit être inférieur à la taile de la dimension correspondante
                        if ($1->index_of_dimensions[i] >= arr->int_array_value->size_of_dimensions[i])
                        {
                                fprintf(stderr, "semantic error : cannot access index %d of array %s which exceeds size the size of its dimension which is %d\n", 
                                $1->index_of_dimensions[i], arr->identifier + 8, arr->int_array_value->size_of_dimensions[i]);
                                exit(1);
                        }
                }

                // calcul de l'adresse de l'élément à accéder
                int address = 0;
                int i = 0;
                for (i = 0; i < arr->int_array_value->number_of_dimensions - 1; i++)
                {
                        address += arr->int_array_value->size_of_dimensions[i] * $1->index_of_dimensions[i];
                }
                address += $1->index_of_dimensions[i];
                address = address * MIPS_REGISTER_SIZE_IN_BYTES;
                // ajout d'un nouveau symbole contenant l'addresse
                struct symbol *addr = symbol_new_temp(&symbol_table);
                addr->int_value = address;
                // génération du quad (QUAD_ARRAY_WRITE)
                struct quad *new_quad = quad_gen(&quad_list, QUAD_ARRAY_WRITE, arr, id, addr, false, NULL);
                $$.result = id;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | TYPE_INT IDENTIFIER ASSIGNMENT expression
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> TYPE_INT IDENTIFIER ASSIGNMENT expression\n");
                // la variable expression doit être un entier (pas de chaines de caractères ou de tableaux/références)
                if ($4.result == NULL || $4.array_value)
                {
                        fprintf(stderr, "semantic error : cannot assign an integer array to the integer variable %s\n", $2 + 8);
                        exit(1);
                }
                if ($4.result->is_string_litteral)
                {
                        fprintf(stderr, "semantic error : cannot assign a string litteral to the integer variable %s\n", $2);
                        exit(1);
                }
                if ($4.result->is_int_array)
                {
                        fprintf(stderr, "semantic error : cannot assign an integer array to the integer variable %s\n", $2);
                        exit(1);
                }
		struct symbol *id = symbol_lookup(symbol_table, $2);
		struct quad *new_quad = quad_gen(&quad_list, QUAD_ASSIGNMENT, id, $4.result, NULL, false, NULL);
                // maintenant l'id est déclaré et initialisé
                id->is_declared = true;
                id->is_set = true;
		$$.result = id;
		$$.code = list_concat($4.code, list_new(new_quad));
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | TYPE_INT IDENTIFIER LEFT_BRACKET RIGHT_BRACKET ASSIGNMENT INT_ARRAY
        {
                if (_verbose_output)
                        printf("declaration_or_assignment -> TYPE_INT IDENTIFIER LEFT_BRACKET RIGHT_BRACKET ASSIGNMENT INT_ARRAY\n");
		struct symbol *id = symbol_lookup(symbol_table, $2);
                // maintenant l'id est déclaré et initialisé
                id->is_declared = true;
                id->is_set = true;
                id->is_int_array = true;
                id->int_array_value = $6;
                // pour chaque case du tableau il faut générer un quad d'affectation (QUAD_ARRAY_WRITE)
                // et concaténer le quad dans la liste contenant le quad (les valeurs sont dans l'ordre)
                struct symbol *address = NULL;
                struct symbol *value = NULL;
                struct quad *new_quad = NULL;
                int index = 0;
                $$.code = NULL;
                for (int i = 0; i < $6->size_of_data; i++)
                {
                        // génération d'un nouveau temporaire contenant l'adresse
                        address = symbol_new_temp(&symbol_table);
                        address->int_value = index;
                        // génération d'un nouveau temporaire contenant la valeur
                        value = symbol_new_temp(&symbol_table);
                        value->int_value = $6->data[i]; 
                        // génération du quad
                        new_quad = quad_gen(&quad_list, QUAD_ARRAY_WRITE, id, value, address, false, NULL);
                        // l'adresse de l'élément suivant est 4 octets après (registres de 32 bits)
                        index += MIPS_REGISTER_SIZE_IN_BYTES;
                        $$.code = list_concat($$.code, list_new(new_quad));
                }
                $$.result = NULL;
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = $6;
        }
        ;

condition:
        condition BOOL_OR condition
        {
                if (_verbose_output)
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
                $$.array_value = NULL;
        }
        | condition BOOL_AND condition
        {
                if (_verbose_output)
                        printf("condition -> condition BOOL_OR condition\n");
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
                $$.array_value = NULL;
        }
        | BOOL_NOT condition
        {
                if (_verbose_output)
                        printf("condition -> BOOL_NOT condition\n");
                // on inverse simplement les truelist et falselist (code et result inchangés)
                $$.result = $2.result;
                $$.code = $2.code;
                $$.truelist = $2.falselist;
                $$.falselist = $2.truelist;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | LEFT_ROUND_BRACKET condition RIGHT_ROUND_BRACKET
        {
                if (_verbose_output)
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
                if (_verbose_output)
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
                $$.array_value = NULL;
        }
        | IDENTIFIER relop IDENTIFIER
        {
                if (_verbose_output)
                        printf("condition -> IDENTIFIER relop IDENTIFIER\n");
                // recherche des deux symboles dans la table
                struct symbol *id1 = symbol_lookup(symbol_table, $1);
                struct symbol *id2 = symbol_lookup(symbol_table, $3);

                // les id doivent être déclarés et initialisés
                if (!id1->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id1->identifier+8);
                        exit(1);
                }
                if (!id1->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id1->identifier+8);
                        exit(1);
                }

                if (!id2->is_declared)
                {
                        fprintf(stderr, "semantic error : %s hasn't been declared previously\n", id2->identifier+8);
                        exit(1);
                }
                if (!id2->is_set)
                {
                        fprintf(stderr, "semantic error : %s hasn't been initialized previously\n", id2->identifier+8);
                        exit(1);
                }

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
                $$.array_value = NULL;
        }
        ;

relop:
        BOOL_EQ
        {
                if (_verbose_output)
                        printf("relop -> BOOL_EQ\n");
                $$ = EQ;
        }
        | BOOL_NE
        {
                if (_verbose_output)
                        printf("relop ->  BOOL_NE\n");
                $$ = NE;
        }
        | BOOL_GT
        {
                if (_verbose_output)
                        printf("relop -> BOOL_GT\n");
                $$ = GT;
        }
        | BOOL_LT
        {
                if (_verbose_output)
                        printf("relop -> BOOL_LT\n");
                $$ = LT;
        }
        | BOOL_GE
        {
                if (_verbose_output)
                        printf("relop -> BOOL_GE\n");
                $$ = GE;
        }
        | BOOL_LE
        {
                if (_verbose_output)
                        printf("relop -> BOOL_LE\n");
                $$ = LE;
        }
        ;

print_function_call:
        PRINT_STRING LEFT_ROUND_BRACKET STRING RIGHT_ROUND_BRACKET
        {
                if (_verbose_output)
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
                $$.array_value = NULL;
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET INTEGER RIGHT_ROUND_BRACKET
        {
                if (_verbose_output)
                        printf("print_function_call -> PRINT_INTEGER LEFT_ROUND_BRACKET INTEGER RIGHT_ROUND_BRACKET\n");
                struct symbol *new = symbol_new_temp(&symbol_table);
                new->int_value = $3;
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, new, NULL, NULL, false, NULL);
                $$.result = NULL;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        | PRINT_INTEGER LEFT_ROUND_BRACKET IDENTIFIER RIGHT_ROUND_BRACKET
        {
                if (_verbose_output)
                        printf("print_function_call -> PRINT_STRING LEFT_ROUND_BRACKET IDENTIFIER RIGHT_ROUND_BRACKET\n");
                struct symbol *id = symbol_lookup(symbol_table, $3);
                struct quad *new_quad = quad_gen(&quad_list, QUAD_PRINTI, id, NULL, NULL, false, NULL);
                $$.result = NULL;
                $$.code = list_new(new_quad);
                $$.truelist = NULL;
                $$.falselist = NULL;
                $$.nextlist = NULL;
                $$.array_value = NULL;
        }
        ;

%%

int main(int argc, char *argv[])
{
        int c;
        opterr = 0;

        while ((c = getopt (argc, argv, "vsqmi:o:")) != -1)
                switch (c)
                {
                        case 'v':
                                _verbose_output = true;
                                break;
                        case 's':
                                _print_symbol_table = true;
                                break;
                        case 'q':
                                _print_quad_list = true;
                                break;
                        case 'm':
                                _mips_verbose = true;
                                break;
                        case 'i':
                                _input_c_file = optarg;
                                break;
                        case 'o':
                                _output_s_file = optarg;
                                break;
                        case '?':
                                if (optopt == 'c')
                                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                                else if (isprint (optopt))
                                        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                                else
                                        fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
                                fprintf(stderr, "usage : %s [-v][-s][-q][-m] -i input_c_file -o output_s_file\n", argv[0]);
                                fprintf(stderr, "-v option prints each grammar rule processed\n");
                                fprintf(stderr, "-s option prints the symbol table at the end of parsing\n");
                                fprintf(stderr, "-q option prints the quad list at the end of parsing\n");
                                fprintf(stderr, "-m option prints some debugging informations during the MIPS assembly code generation\n");
                                fprintf(stderr, "-i file.c indicates in which file the compiler reads the C code\n");
                                fprintf(stderr, "-o file.s indicates in which file the compiler writes the MIPS assembly code\n");
                                return 1;
                        default:
                                abort ();
                }

        // ouverture du fichier
        FILE *input = fopen(_input_c_file, "r");
        yyin = input;

        yyparse();

        // affichage de la table des symboles (option -s)
        if (_print_symbol_table)
                symbol_print(symbol_table);
        // affichage de la liste de quads (option -q)
        if (_print_quad_list)
                quad_print(quad_list);

        fclose(input);

        // écriture du code MIPS dans le fichier _output_s_file (option -o)
        struct mips_generator *mips = mips_setup(_output_s_file, symbol_table, quad_list);
        mips_generate_assembly_code(mips);

        return 0;
}
