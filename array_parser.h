#ifndef __ARRAY_PARSER_H__
#define __ARRAY_PARSER_H__

#include <stdbool.h>

// structure permettant de prendre en compte à la fois les indices entiers et les variables
struct stenc_index
{
	bool is_identifier;
	union {
		char *identifier;
		int value;
	};
};

// structure représentant les tableaux ou les références de tableaux
struct stenc_array
{
	char *identifier;		// nom du tableau (tableau et références)
	int number_of_dimensions;	// nombre de dimensions (tableau et références)
	int *size_of_dimensions;	// taille de chaque dimension (tableau uniquement)
	int size_of_data;		// nombre de valeurs (tableau et références quand affectation de tableau)
	int *data;			// valeurs (tableau uniquement)
	// permet de représenter les indices (entier et variablres)
	struct stenc_index *index_of_dimensions;
};

// fonctions de manipulation des valeurs de tableau
bool __array_parser_check_braces(char *);
bool __array_parser_check_commas(char *);
int __array_parser_count_values(char *);
int __array_parser_count_number_of_dimensions(char *);
void __array_parser_depth_block_count(char *, int, int *);
int *__array_parser_count_size_of_dimensions(char *, int);
struct stenc_array *array_parser_parse_text(char *);

// fonctions de manipulation des références de tableau
char *__array_parser_extract_reference_identifier(char *);
int __array_parser_count_reference_dimensions(char *);
struct stenc_index *__array_parser_count_index_of_dimensions(char *, int);
struct stenc_array *array_parser_parse_reference(char *);

void array_parser_free(struct stenc_array *);
void array_parser_print(struct stenc_array *);

#endif
