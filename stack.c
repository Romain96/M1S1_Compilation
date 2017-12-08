#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// alloue la m�moire pour une nouvelle pile
struct stack *stack_new()
{
	struct stack *s = (struct stack *)malloc(sizeof(struct stack));
	if (s == NULL)
	{
		fprintf(stderr, "[STACK::stack_new] ERROR while allocating struct stack\n");
		exit(EXIT_FAILURE);
	}

	// la t�te est vide
	s->head = NULL;
	return s;
}

// ajoute la valeur dans la pile (ajout en t�te)
void stack_push(struct stack *s, int value)
{
	// allocation d'un nouveau stack_element qui devient la nouvelle t�te de pile
	struct stack_element *elt = (struct stack_element *)malloc(sizeof(struct stack_element));
	if (elt == NULL)
	{
		fprintf(stderr, "[STACK : stack_push] ERROR while allocating struct stack\n");
		exit(EXIT_FAILURE);
	}

	elt->value = value;
	// la t�te courante est elt->next
	elt->next = s->head;
	// et elt est la nouvelle t�te
	s->head = elt;
}

// retire l'�lement en t�te (retourne sa valeur et lib�re la m�moire allou�e pour lui)
int stack_pop(struct stack *s)
{
	if (s->head == NULL)
	{
		fprintf(stderr, "[STACK::stack_pop] ERROR stack head is NULL (empty stack ?)\n");
		exit(EXIT_FAILURE);
	}

	// conservation du pointeur next
	struct stack_element *temp = s->head->next;
	// et de la valeur de la t�te
	int return_value = s->head->value;
	// lib�ration de la m�moire
	free(s->head);
	// temp est la nouvelle t�te d�sormais
	s->head = temp;
		
	return return_value;
}

// comportement similaire � stack_pop sauf que l'�lement n'est pas supprim� de la pile
int stack_head(struct stack *s)
{
	return s->head->value;
}

// v�rifie que la pile est vide
bool stack_is_empty(struct stack *s)
{
	if (s->head == NULL)
		return true;
	else
		return false;
}

// lib�re la m�moire allou�e pour la pile
void stack_free(struct stack *s)
{
	// lib�ration des donn�es de type stack_element
	if (s->head != NULL)
	{
		struct stack_element *iterator = s->head;
		struct stack_element *elt_to_free = s->head;
		while (iterator != NULL)
		{
			elt_to_free = iterator;
			free(elt_to_free);
			iterator = iterator->next;
		}
		free(s);
	}
}

// DEBUG : affiche le contenu de la pile
void stack_print(struct stack *s)
{
	struct stack_element *iterator = s->head;
	while (iterator != NULL)
	{
		printf("%d\n", iterator->value);
		iterator = iterator->next;
	}
}