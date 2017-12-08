#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>

// structure
struct stack_element
{
	int value;
	struct stack_element *next;
};

struct stack
{
	struct stack_element *head;
};

// fonctions
struct stack *stack_new();
void stack_push(struct stack *, int);
int stack_pop(struct stack *);
int stack_head(struct stack *);
bool stack_is_empty(struct stack *);
void stack_free(struct stack *);
void stack_print(struct stack *);

#endif