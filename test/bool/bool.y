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
        char *string;
        struct codegen {
                struct list *code;
                struct list *truelist;
                struct list *falselist;
        }gencode;
        enum binary_op{RELOP_EQ, RELOP_NE, RELOP_GT, RELOP_LT, RELOP_GE, RELOP_LE} op;
}

%token <string> IDENTIFIER
%token AND OR NOT
%token EQ NE GT LT GE LE

%type <gencode> condition
%type <gencode> line
%type <op> relop

%left AND OR
%left EQ NE GT GE LT LE

%start line

%%

line:
        condition '\n'
        {
                printf("line -> condition\n");
                $$.code = $1.code;
                $$.truelist = $1.truelist;
                $$.falselist = $1.falselist;
                printf("Match !!!\n");
                return 0;
        }
        ;

condition:
        condition OR condition
        {
                printf("line -> condition OR condition\n");
                list_complete($1.falselist, $3.code->current_quad->id);
                $$.falselist = $3.falselist;
                $$.truelist = list_concat($1.truelist, $3.truelist);
                $$.code = list_concat($1.code, $3.code);
        }
        | condition AND condition
        {
                printf("line -> condition AND condition\n");
                list_complete($1.truelist, $3.code->current_quad->id);
                $$.falselist = list_concat($1.falselist, $3.falselist);
                $$.truelist = $3.truelist;
                $$.code = list_concat($1.code, $3.code);
        }
        | NOT condition
        {
                printf("line -> NOT condition\n");
                $$.code = $2.code;
                $$.truelist = $2.falselist;
                $$.falselist = $2.truelist;
        }
        | '(' condition ')'
        {
                printf("line -> ( condition )\n");
                $$.code = $2.code;
                $$.truelist = $2.truelist;
                $$.falselist = $2.falselist;
        }
        | IDENTIFIER
        {
                printf("line -> IDENTIFIER\n");
                struct symbol *id = symbol_lookup(symbol_table, $1);
                $$.truelist = list_new(quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, id, true, -1 ));
                $$.falselist = list_new(quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, -1));
                $$.code = list_concat ($$.truelist, $$.falselist);
        }
        | IDENTIFIER relop IDENTIFIER
        {
                printf("line -> IDENTIFIER relop IDENTIFIER\n");
                struct quad *new_quad1 = NULL;
                struct quad *new_quad2 = NULL;
                struct symbol *id1 = symbol_lookup(symbol_table, $1);
                struct symbol *id2 = symbol_lookup(symbol_table, $3);
                // goto conditionnel
                switch ($2)
                {
                        case RELOP_EQ:
                                new_quad1 = quad_gen(&quad_list, QUAD_EQ, id1, id2, NULL, true, -1);
                                break;
                        case RELOP_NE:
                                new_quad1 = quad_gen(&quad_list, QUAD_NE, id1, id2, NULL, true, -1);
                                break;
                        case RELOP_GT:
                                new_quad1 = quad_gen(&quad_list, QUAD_GT, id1, id2, NULL, true, -1);
                                break;
                        case RELOP_LT:
                                new_quad1 = quad_gen(&quad_list, QUAD_LT, id1, id2, NULL, true, -1);
                                break;
                        case RELOP_GE:
                                new_quad1 = quad_gen(&quad_list, QUAD_GE, id1, id2, NULL, true, -1);
                                break;
                        case RELOP_LE:
                                new_quad1 = quad_gen(&quad_list, QUAD_LE, id1, id2, NULL, true, -1);
                                break;
                        default:
                                break;
                }
                // goto inconditionnel
                new_quad2 = quad_gen(&quad_list, QUAD_NO_OP, NULL, NULL, NULL, true, -1);

                // gestion des listes
                $$.truelist = list_new(new_quad1);
                $$.falselist = list_new(new_quad2);
                $$.code = list_concat($$.truelist, $$.falselist);
        }
        ;

relop:
        EQ
        {
                $$ = RELOP_EQ;
        }
        | NE
        {
                $$ = RELOP_NE;
        }
        | GT
        {
                $$ = RELOP_GT;
        }
        | LT
        {
                $$ = RELOP_LT;
        }
        | GE
        {
                $$ = RELOP_GE;
        }
        | LE
        {
                $$ = RELOP_LE;
        }
        ;

%%

int main()
{
        printf("Enter a boolean condition\n");
        yyparse();
        symbol_print(symbol_table);
        quad_print(quad_list);
        return 0;
}