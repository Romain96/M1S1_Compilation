#ifndef __ARRAY_PARSER_H__
#define __ARRAY_PARSER_H__

#include <stdbool.h>

// structure
struct int_array
{
	int number_of_dimensions;
	int *size_of_dimensions;
	int size_of_data;
	int *data;
};

// fonctions de parsing des valeurs ({...})
bool __array_parser_check_braces(char *);
bool __array_parser_check_commas(char *);
int __array_parser_count_values(char *);
int __array_parser_count_number_of_dimensions(char *);
void __array_parser_depth_block_count(char *, int, int *);
int *__array_parser_count_size_of_dimensions(char *, int);
struct int_array *array_parser_parse_text(char *);
void array_parser_free(struct int_array *);
void array_parser_print_struct(struct int_array *);
// fonctions de parsing des identificateurs de tableaux
char *array_parser_extract_identifier(char *);

#endif
