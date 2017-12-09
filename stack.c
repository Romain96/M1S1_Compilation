#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Fonction             : stack_new
// Argument(s)          : /
// Valeur de retour     : un pointeur sur une nouvelle structure stack allouée par la fonction
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : alloue la mémoire pour une nouvelle pile
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

// Fonction             : stack_push
// Argument(s)          : - s : un pointeur sur une structure stack
//			  - value : l'entier à ajouter
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : ajoute la valeur dans la pile (ajout en tête)
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

// Fonction             : stack_pop
// Argument(s)          : - s : un pointeur sur une structure stack
// Valeur de retour     : un entier contenant la valeur de la tête de la pile
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : retire l'élement en tête (retourne sa valeur et libère la mémoire allouée pour lui)
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

// Fonction             : stack_head
// Argument(s)          : - s : un pointeur sur une structure stack
// Valeur de retour     : la valeur de la tête de pile
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : comportement similaire à stack_pop sauf que l'élement n'est pas supprimé de la pile
int stack_head(struct stack *s)
{
	return s->head->value;
}

// Fonction             : stack_is_empty
// Argument(s)          : - s : un pointeur sur une structure stack
// Valeur de retour     : vrai si la pile est vide, faux sinon
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)      	: vérifie que la pile est vide
bool stack_is_empty(struct stack *s)
{
	if (s->head == NULL)
		return true;
	else
		return false;
}

// Fonction             : stack_free
// Argument(s)          : - s : un pointeur sur une structure stack
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : libère la mémoire allouée pour la pile
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

// Fonction             : stack_print
// Argument(s)          : - s : un pointeur sur une structure stack
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : affiche le contenu de la pile
void stack_print(struct stack *s)
{
	struct stack_element *iterator = s->head;
	while (iterator != NULL)
	{
		printf("%d\n", iterator->value);
		iterator = iterator->next;
	}
}