#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdbool.h>

// structure de table des symboles sous forme de liste chaînée
struct symbol 
{
	char *identifier;
	bool is_constant;
	int value;
	struct symbol *next;
};

// fonctions de manipulation de la table des symboles
struct symbol *symbol_alloc();
struct symbol *symbol_new_temp(struct symbol **);
struct symbol *symbol_lookup(struct symbol *, char *);
struct symbol *symbol_add(struct symbol **, char *);
void symbol_print(struct symbol *);

#endif
