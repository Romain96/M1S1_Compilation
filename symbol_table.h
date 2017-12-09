#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdbool.h>
#include "array_parser.h"

// structure de table des symboles sous forme de liste chainée
struct symbol 
{
	char *identifier;
	bool is_constant;
	bool is_declared;
	bool is_set;
	bool is_string_litteral;
	bool is_int_array;
	union {
		int int_value;		// prend en compte les variables entières
		char *string_value;	// prend en compte les chaines de caractères
		struct stenc_array *int_array_value;	// prend en compte les tableaux multidimensionnels et les stencils
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
