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
	new->is_declared = false;
	new->is_set = false;
	new->is_string_litteral = false;
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
		// trouvé -> on retourne le symbole correspondant
		if (strcmp(table->identifier, name) == 0)
			return table;
		table = table->next;
	}
	// non trouvé
	return NULL;
}

// libère la mémoire allouée pour la table des symboles
void symbol_free(struct symbol **symbol)
{
	struct symbol *iterator = *symbol;
	struct symbol *symbol_to_free = *symbol;

	// parcours de tous les symboles
	while (iterator != NULL)
	{
		iterator = iterator->next;
		free(symbol_to_free->identifier);
		free(symbol_to_free);
		symbol_to_free = iterator;
	}
}

// Affiche la table des symboles
void symbol_print(struct symbol *symbol)
{
	printf("\n///////////////////////////////////////////////////////////\n");
	printf("/////////////////////  Symbol table  //////////////////////\n");
	printf("///////////////////////////////////////////////////////////\n");
	while (symbol != NULL)
	{
		printf("id : %10s, is_constant : ", symbol->identifier);
		if (symbol->is_constant)
			printf("true, ");
		else
			printf("false, ");
		if (symbol->is_declared)
			printf("is_declared : true, ");
		else
			printf("is_declared : false :");
		if (symbol->is_declared)
			printf("is_set : true, ");
		else
			printf("is_set : false :");
		if (symbol->is_string_litteral)
			printf("string value : %s\n", symbol->string_value);
		else
			printf("int value : %d\n", symbol->int_value);
		symbol = symbol->next;
	}
}
