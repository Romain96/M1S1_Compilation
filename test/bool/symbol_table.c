#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define SYMBOL_MAX_STRING 42

// Alloue un nouveau symbole
struct symbol *symbol_alloc()
{
	struct symbol *new = malloc(sizeof(struct symbol));
	new->identifier = NULL;
	new->is_constant = false;
	new->value = 0;
	new->next = NULL;
	return new;
}

// Ajoute le symbole dans la table
struct symbol *symbol_add(struct symbol **table, char *name)
{
	// 1er élément de la table (si la table est vide)
	if (*table == NULL)
	{
		*table = symbol_alloc();
		(*table)->identifier = strdup(name);
		return *table;
	}
	else
	{
		struct symbol *scan = *table;
		// parcours de la liste chainée jusqu'à la fin
		while (scan->next != NULL)
			scan = scan->next;
		scan->next = symbol_alloc();
		scan->next->identifier = strdup(name);
		return scan->next;
	}
}

// Crée un nouveau temporaire (utilise symbol_add)
struct symbol *symbol_new_temp(struct symbol **table)
{
	static int temporary_number = 0;
	char temporary_name[SYMBOL_MAX_STRING];
	snprintf(temporary_name, SYMBOL_MAX_STRING, "temp%d", temporary_number);
	temporary_number++;
	return symbol_add(table, temporary_name);
}

// Recherche le symbole dans la table
struct symbol *symbol_lookup(struct symbol *table, char *name)
{
	while (table != NULL)
	{
		// trouv� -> on retourne le symbole correspondant
		if (strcmp(table->identifier, name) == 0)
			return table;
		table = table->next;
	}
	// non trouv�
	return NULL;
}

// Affiche la table des symboles
void symbol_print(struct symbol *symbol)
{
	printf("/////////////////////////////////////////////////\n");
	printf("////////////////  Symbol table  /////////////////\n");
	printf("/////////////////////////////////////////////////\n");
	while (symbol != NULL)
	{
		printf("id : %10s, is_constant : ", symbol->identifier);
		if (symbol->is_constant)
			printf("true, value : %d\n", symbol->value);
		else
			printf("false, value : N/A\n");
		symbol = symbol->next;
	}
}
