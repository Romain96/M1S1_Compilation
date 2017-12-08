#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "array_parser.h"

#define AP_LEFT_BRACE 1
#define AP_RIGHT_BRACE 2
#define INTEGER_DIGIT_MAX_SIZE 42

// v�rifie qu'il y a autant de '{' que de '}' et que l'ordre est respect�
bool __array_parser_check_braces(char *text)
{
	struct stack *s = stack_new();

	// conserve '{' et '}' dans une pile et v�rifie que '}' correspond bien � une t�te contenant '{'
	for (unsigned int i = 0; i < strlen(text) + 1; i++)
	{
		if (text[i] == '{')
			stack_push(s, AP_LEFT_BRACE);
		if (text[i] == '}')
		{
			if (stack_head(s) != AP_LEFT_BRACE)
				return false;
			else
			{
				if (stack_is_empty(s) == true)
					return false;
				int tmp = stack_pop(s);
			}
		}
	}

	// � la fin la pile devrait �tre vide
	if (s->head == NULL)
	{
		stack_free(s);
		return true;
	}
	else
	{
		stack_free(s);
		return false;
	}
}

// v�rifie que les virgules sont toujours entre des nombres (ou entre '}' et '{')
bool __array_parser_check_commas(char *text)
{
	// taille minimale requise est {nombre_de_un_chiffre} (3)
	if (strlen(text) <= 3)
		return false;

	//conserve '{' et '}' dans une pile et v�rifie que ',' n'est pas apr�s { ou avant }
	for (unsigned int i = 1; i < strlen(text); i++)
	{
		if (text[i] == ',')
		{
			if (text[i-1] == '{')
				return false;
		}
		if (text[i] == '}')
		{
			if (text[i-1] == ',')
				return false;
			if (text[i + 1] == '{')
				return false;
		}
	}

	return true;
}

// compte le nombre d'entiers dans la chaine
int __array_parser_count_values(char *text)
{
	int i = 0;
	int number_of_values = 0;
	while (i < strlen(text) + 1)
	{
		// detection du premier chiffre (ou -)
		if (text[i] == '-' || text[i] == '0' || text[i] == '1' || text[i] == '2' || text[i] == '3' || text[i] == '4' || 
			text[i] == '5' || text[i] == '6' || text[i] == '7' || text[i] == '8' || text[i] == '9')
		{
			number_of_values++;
			// chercher la fin du nombre
			while (i < strlen(text) + 1 && (text[i] == '-' || text[i] == '0' || text[i] == '1' || text[i] == '2' || text[i] == '3' || 
				text[i] == '4' || text[i] == '5' || text[i] == '6' || text[i] == '7' || text[i] == '8' || text[i] == '9'))
				i++;
		}
		else
			i++;
	}
	return number_of_values;
}

// compte le nombre de dimensions (profondeur maximale de {{{{... en partant de 0)
int __array_parser_count_number_of_dimensions(char *text)
{
	size_t i = 0;
	int number_of_dimensions = 0;

	while (i < strlen(text) + 1)
	{
		if (text[i] == '{')
			number_of_dimensions++;
		if (text[i] == '}')
			return number_of_dimensions;
		i++;
	}
	return number_of_dimensions;
}

// compte le nombre de valeurs d'une dimension (repr�sent�e comme une profondeur d'accolade)
void __array_parser_depth_block_count(char *text, int dim, int *dimsize)
{
	int comma_depth = 0;
	int i = 0;

	while (i < strlen(text) + 1)
	{
		if (text[i] == '{')
		{
			comma_depth++;
		}
		if (text[i] == ',')
		{
			if (comma_depth == dim)
				dimsize[dim - 1]++;
		}
		if (text[i] == '}')
		{
			if (comma_depth == dim)
			{
				dimsize[dim - 1]++;
				return;
			}
			comma_depth--;
		}
		i++;
	}
}

// compte la taille de chaque dimension (ndim devrait �tre obtenu avec __array_parser_count_number_of_dimensions)
int *__array_parser_count_size_of_dimensions(char *text, int ndim)
{
	// allocation de la m�moire pour les dimensions
	int *dimsize = (int *)malloc(ndim * sizeof(int));
	if (dimsize == NULL)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_count_size_of_dimensions] error while allocating array\n");
		exit(EXIT_FAILURE);
	}

	// initialisation � 0 pour chaque dimension
	for (int i = 0; i < ndim; i++)
		dimsize[i] = 0;

	for (int i = 0; i < ndim; i++)
		__array_parser_depth_block_count(text, i + 1, dimsize);

	return dimsize;
}

// tente de parser toutes la valeurs dans la structure int_array
struct int_array *array_parser_parse_text(char *text)
{
	// v�rification des accolades
	if (__array_parser_check_braces(text) == false)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_parse_text] Braces checking function __array_parser_check_braces returned false\n");
		exit(EXIT_FAILURE);
	}

	// v�rification des virgules
	if (__array_parser_check_commas(text) == false)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_parse_text] Commas checking function __array_parser_check_commas returned false\n");
		exit(EXIT_FAILURE);
	}

	// comptage du nombre de valeurs
	int nval = __array_parser_count_values(text);

	// comptage du nombre de dimensions
	int ndim = __array_parser_count_number_of_dimensions(text);

	// allocation de la m�moire pour la structure int_array
	struct int_array *arr = (struct int_array *)malloc(sizeof(struct int_array));
	// et ses donn�es
	arr->data = (int *)malloc(nval * sizeof(int));
	arr->number_of_dimensions = ndim;
	arr->size_of_data = nval;

	// comptage de la taille de chaque dimension
	int *dimsize = __array_parser_count_size_of_dimensions(text, ndim);
	arr->size_of_dimensions = dimsize;

	// parsing de la chaine en entier
	char integer[INTEGER_DIGIT_MAX_SIZE];
	int i = 0;
	int integer_begin = 0;
	int integer_end = 0;
	int value_number = 0;
	while (i < strlen(text) + 1)
	{
		// premier chiffre (ou -)
		if (text[i] == '-' || text[i] == '0' || text[i] == '1' || text[i] == '2' || text[i] == '3' || text[i] == '4' ||
			text[i] == '5' || text[i] == '6' || text[i] == '7' || text[i] == '8' || text[i] == '9')
		{
			integer_begin = i;
			// chercher la fin du nombre courant
			while (i < strlen(text) + 1 && (text[i] == '-' ||  text[i] == '0' || text[i] == '1' || text[i] == '2' || text[i] == '3' || 
				text[i] == '4' || text[i] == '5' || text[i] == '6' || text[i] == '7' || text[i] == '8' || text[i] == '9'))
				i++;
			integer_end = i;

			// extraction de l'entier
			snprintf(integer, integer_end - integer_begin + 1, "%s", text + integer_begin);
			
			// placement dans la structure
			arr->data[value_number] = atoi(integer);
			value_number++;
		}
		else
			i++;
	}
	return arr;
}

// lib�re la m�moire allou�e par la structure
void array_parser_free(struct int_array *arr)
{
	// lib�re les tailles de dimensions
	free(arr->size_of_dimensions);
	// lib�re les donn�es
	free(arr->data);
	// lib�re la structure
	free(arr);
}

// DEBUG : affiche le contenu de la structure int_array
void array_parser_print_struct(struct int_array *arr)
{
	printf("INT_ARRAY_STRUCT\n");
	printf("Number of dimensions is %d\n", arr->number_of_dimensions);
	for (int i = 0; i < arr->number_of_dimensions; i++)
		printf("Dimension %d is of size %d\n", i, arr->size_of_dimensions[i]);
	printf("Number of values is %d\n", arr->size_of_data);
	for (int i = 0; i < arr->size_of_data; i++)
		printf("Integer %d : %d\n", i, arr->data[i]);
}

// extrait le nom du tableau
char *array_parser_extract_identifier(char *text)
{
	int i = 0;
	int id_begin = 0;
	int id_end = 0;

	while(i < strlen(text) + 1)
	{
		// première lettre (a-zA-Z_)
		if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122) || text[i] == '_')
		{
			// chercher la fin
			id_begin = i;
			while (i < strlen(text) + 1 && (text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122) || text[i] == '_')
				i++;
			id_end = i;
			
			// extraction
			char *id = malloc((id_end - id_begin + 1) * sizeof(char));
			if (id == NULL)
			{
				fprintf(stderr, "[ARRAY_PARSER::array_parser_extract_identifier] ERROR while allocating id\n");
				exit(EXIT_FAILURE);
			}
			strncpy(id, text + id_begin, (id_end - id_begin));
			id[id_end - id_begin] = '\0';	// strncpy n'écrit pas '\0' si la chaine est plus longue
			printf("array id is %s\n", id);
			return id;
		}
	}
	return NULL;
}
