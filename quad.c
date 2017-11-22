#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "quad.h"

// génère un nouveau quad
struct quad *quad_gen(struct quad **quad_list, enum quad_operator op, struct symbol *arg1, struct symbol *arg2, struct symbol *res)
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
		(*quad_list)->next = NULL;
		quad_number++;
		return *quad_list;
	}
	else
	{
		// recherche du dernier quad
		struct quad *scan = *quad_list;
		while (scan->next != NULL)
			scan = scan->next;
		scan->next = malloc(sizeof(struct quad));
		scan->next->id = quad_number;
		scan->next->op = op;
		scan->next->arg1 = arg1;
		scan->next->arg2 = arg2;
		scan->next->res = res;
		scan->next->next = NULL;
		quad_number++;
		return (*quad_list)->next;
	}
}

// concatène deux listes de quads
struct quad *quad_add(struct quad *quad_list1, struct quad *quad_list2)
{
	// crée une nouvelle liste
	struct quad *concat = NULL;

	// ajout des quads de la liste 1
	struct quad *iterator = quad_list1;
	while (iterator->next != NULL)
	{
		quad_gen(&concat, iterator->op, iterator->arg1, iterator->arg2, iterator->res);
		iterator = iterator->next;
	}

	// ajout des quads de la liste 2
	iterator = quad_list2;
	while (iterator->next != NULL)
	{
		quad_gen(&concat, iterator->op, iterator->arg1, iterator->arg2, iterator->res);
		iterator = iterator->next;
	}

	return concat;
}

// affiche la liste de quads
void quad_print(struct quad *quad_list)
{
	printf("/////////////////////////////////////////////////\n");
	printf("////////////////// Quads list ///////////////////\n");
	printf("/////////////////////////////////////////////////\n");
	while (quad_list != NULL)
	{
		printf("id : %5d, operator : ", quad_list->id);

		// affichage de l'opérateur
		switch (quad_list->op)
		{
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
			case QUAD_GOTO:
				printf("goto, "); break;
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
			printf("result : NULL\n");
		else
			printf("result : %p\n", quad_list->res);

		quad_list = quad_list->next;
	}
}
