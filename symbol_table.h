#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdbool.h>

// structure de table des symboles sous forme de liste cha�n�e
struct symbol 
{
	char *identifier;
	bool is_constant;
	bool is_string_litteral;
	union {
		int int_value;		// prend en compte les variables entères
		char *string_value;	// prend en compte les chaines de caractères
	};
	struct symbol *next;
};

// fonctions de manipulation de la table des symboles
struct symbol *symbol_alloc();
struct symbol *symbol_new_temp(struct symbol **);
struct symbol *symbol_lookup(struct symbol *, char *);
struct symbol *symbol_add(struct symbol **, char *);
void symbol_free(struct symbol **);
void symbol_print(struct symbol *);

#endif
