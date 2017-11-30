#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "quad.h"

#define QUAD_MAX_LABEL_STRING 42

// génère un nouveau quad
struct quad *quad_gen(struct quad **quad_list, enum quad_operator op, struct symbol *arg1, struct symbol *arg2, struct symbol *res, bool contain_goto, char *goto_quad)
{
	static int quad_number = 0;

	// 1er élément de la liste
	if (*quad_list == NULL)
	{
		*quad_list = malloc(sizeof(struct quad));
		(*quad_list)->id = quad_number;
		(*quad_list)->op = op;
		(*quad_list)->arg1 = arg1;
		(*quad_list)->arg2 = arg2;
		(*quad_list)->res = res;
		(*quad_list)->contain_goto = contain_goto;
		(*quad_list)->goto_quad = goto_quad;
		(*quad_list)->is_labelled = false;
		(*quad_list)->label_name = NULL;
		(*quad_list)->next = NULL;
		quad_number++;
		return *quad_list;
	}
	else
	{
		// allocation d'un nouveau quad
		struct quad *new_quad = malloc(sizeof(struct quad));
		new_quad->id = quad_number;
		new_quad->op = op;
		new_quad->arg1 = arg1;
		new_quad->arg2 = arg2;
		new_quad->res = res;
		new_quad->contain_goto = contain_goto;
		new_quad->goto_quad = goto_quad;
		new_quad->is_labelled = false;
		new_quad->label_name = NULL;
		new_quad->next = NULL;
		quad_number++;

		// recherche du dernier quad
		struct quad *scan = *quad_list;
		while (scan->next != NULL)
			scan = scan->next;

		scan->next = new_quad;
		return scan->next;
	}
}

// étiquette un quad
void quad_label(struct quad *q)
{
	static int label_number = 0;
	char label[QUAD_MAX_LABEL_STRING];
	snprintf(label, QUAD_MAX_LABEL_STRING, "label_%d", label_number);
	label_number++;
	q->is_labelled = true;
	q->label_name = strdup(label);
}

// libère la mémoire allouée par la liste de quads
void quad_free(struct quad **ql)
{
	struct quad *iterator = *ql;
	struct quad *quad_to_free = *ql;
	while (iterator != NULL)
	{
		iterator = iterator->next;
		if (quad_to_free->contain_goto)
			free(quad_to_free->goto_quad);
		if (quad_to_free->is_labelled)
			free(quad_to_free->label_name);
		free(quad_to_free);
		quad_to_free = iterator;
	}
}

// affiche la liste de quads
void quad_print(struct quad *quad_list)
{
	printf("\n/////////////////////////////////////////////////////////////////////////////////////////////\n");
	printf("///////////////////////////////////////// Quad list /////////////////////////////////////////\n");
	printf("/////////////////////////////////////////////////////////////////////////////////////////////\n");
	while (quad_list != NULL)
	{
		printf("id : %5d, operator : ", quad_list->id);

		// affichage de l'opérateur
		switch (quad_list->op)
		{
			case QUAD_NO_OP:
				printf("N/A, "); break;
			case QUAD_PLUS:
				printf("+, "); break;
			case QUAD_MINUS:
				printf("-, "); break;
			case QUAD_MULTIPLY:
				printf("*, "); break;
			case QUAD_DIVIDE:
				printf("/, ");break;
			case QUAD_EQ:
				printf("==, "); break;
			case QUAD_NE:
				printf("!=, "); break;
			case QUAD_GT:
				printf(">, "); break;
			case QUAD_LT:
				printf("<, "); break;
			case QUAD_GE:
				printf(">=, "); break;
			case QUAD_LE:
				printf("<=, "); break;
			case QUAD_ASSIGNMENT:
				printf("=, "); break;
			case QUAD_PRINTF:
				printf("printf, "); break;
			case QUAD_PRINTI:
				printf("printi, "); break;
			default:
				printf("N/A, "); break;
		}


		// affichage des pointeurs (ou NULL)
		if (quad_list->arg1 == NULL)
			printf("arg1 : NULL, ");
		else
			printf("arg1 : %p, ", quad_list->arg1);

		if (quad_list->arg2 == NULL)
			printf("arg2 : NULL, ");
		else
			printf("arg2 : %p, ", quad_list->arg2);

		if (quad_list->res == NULL)
			printf("result : NULL");
		else
			printf("result : %p", quad_list->res);

		// affiche le goto s'il y en a un
		if (quad_list->contain_goto)
			printf(", goto : %s", quad_list->goto_quad);

		// affichage du label s'il en a un
		if (quad_list->is_labelled)
			printf(", label : %s\n", quad_list->label_name);
		else
			printf("\n");

		quad_list = quad_list->next;
	}
}
