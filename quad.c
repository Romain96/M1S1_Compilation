#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "quad.h"

// génère un nouveau quad
struct quad *quad_gen(struct quad **quad_list, enum quad_operator op, struct symbol *arg1, struct symbol *arg2, struct symbol *res, bool contain_goto, int goto_quad)
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

// concatène deux listes de quads
struct quad *quad_concat(struct quad *l1, struct quad *l2)
{
	// si les deux listes sont vide on retourne une liste vide
	if (l1 == NULL && l2 == NULL)
		return NULL;

	// si l1 est vide on retourne l2
	if (l1 == NULL)
		return l2;

	// si l2 est vide on retourne l1
	if (l2 == NULL)
		return l1;

	// sinon on parcours l1 jusqu'à la fin et on ajoute un lien vers l2
	struct quad *iterator = l1;
	while (iterator->next != NULL)
		iterator = iterator->next;
	iterator->next = l2;
	// et on retourne l1 qui désormais contient l1 + l2
	return l1;
}

// libère la mémoire allouée par la liste de quads
void quad_free(struct quad **ql)
{
	struct quad *iterator = *ql;
	struct quad *quad_to_free = *ql;
	while (iterator != NULL)
	{
		iterator = iterator->next;
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
		if (quad_list->contain_goto == true)
			printf(", goto : %d\n", quad_list->goto_quad);
		else
			printf("\n");

		quad_list = quad_list->next;
	}
}
