#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// alloue la mémoire pour une nouvelle pile
struct stack *stack_new()
{
	struct stack *s = (struct stack *)malloc(sizeof(struct stack));
	if (s == NULL)
	{
		fprintf(stderr, "[STACK::stack_new] ERROR while allocating struct stack\n");
		exit(EXIT_FAILURE);
	}

	// la tête est vide
	s->head = NULL;
	return s;
}

// ajoute la valeur dans la pile (ajout en tête)
void stack_push(struct stack *s, int value)
{
	// allocation d'un nouveau stack_element qui devient la nouvelle tête de pile
	struct stack_element *elt = (struct stack_element *)malloc(sizeof(struct stack_element));
	if (elt == NULL)
	{
		fprintf(stderr, "[STACK : stack_push] ERROR while allocating struct stack\n");
		exit(EXIT_FAILURE);
	}

	elt->value = value;
	// la tête courante est elt->next
	elt->next = s->head;
	// et elt est la nouvelle tête
	s->head = elt;
}

// retire l'élement en tête (retourne sa valeur et libère la mémoire allouée pour lui)
int stack_pop(struct stack *s)
{
	if (s->head == NULL)
	{
		fprintf(stderr, "[STACK::stack_pop] ERROR stack head is NULL (empty stack ?)\n");
		exit(EXIT_FAILURE);
	}

	// conservation du pointeur next
	struct stack_element *temp = s->head->next;
	// et de la valeur de la tête
	int return_value = s->head->value;
	// libération de la mémoire
	free(s->head);
	// temp est la nouvelle tête désormais
	s->head = temp;
		
	return return_value;
}

// comportement similaire à stack_pop sauf que l'élement n'est pas supprimé de la pile
int stack_head(struct stack *s)
{
	return s->head->value;
}

// vérifie que la pile est vide
bool stack_is_empty(struct stack *s)
{
	if (s->head == NULL)
		return true;
	else
		return false;
}

// libère la mémoire allouée pour la pile
void stack_free(struct stack *s)
{
	// libération des données de type stack_element
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