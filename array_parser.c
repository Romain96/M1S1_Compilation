#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "array_parser.h"

#define AP_LEFT_BRACE 1
#define AP_RIGHT_BRACE 2
#define INTEGER_DIGIT_MAX_SIZE 42

// Fonction             : __array_parser_check_braces
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : vrai si la chaine est bien formée, faux sinon
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : vérifie qu'il y a autant de '{' que de '}' et que l'ordre est respecté
bool __array_parser_check_braces(char *text)
{
	struct stack *s = stack_new();

	// conserve '{' et '}' dans une pile et vérifie que '}' correspond bien à une tête contenant '{'
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

	// à la fin la pile devrait être vide
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

// Fonction             : __array_parser_check_commas
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : vrai si la chaine est bien formée, faux sinon
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : vérifie que les virgules sont toujours entre des nombres (ou entre '}' et '{')
bool __array_parser_check_commas(char *text)
{
	// taille minimale requise est {nombre_de_un_chiffre} (3)
	if (strlen(text) <= 3)
		return false;

	//conserve '{' et '}' dans une pile et vérifie que ',' n'est pas après { ou avant }
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

// Fonction             : __array_parser_count_values
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : le nombre de valeurs dans la chaine
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : compte le nombre d'entiers dans la chaine
int __array_parser_count_values(char *text)
{
	int i = 0;
	int number_of_values = 0;
	while (i < strlen(text) + 1)
	{
		// détection du premier chiffre (ou -)
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

// Fonction             : __array_parser_count_number_of_dimensions
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : le nombre de dimensions de la chaine
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : compte le nombre de dimensions (profondeur maximale de {{{{... en partant de 0)
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

// Fonction             : __array_parser_depth_block_count
// Argument(s)          : - text : la chaine de caractères à parser
//			  - dim : la dimensions de départ
//			  - dimsize : un tableau à remplir avec la taille des dimensions
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : compte le nombre de valeurs d'une dimension (représentée comme une profondeur d'accolade)
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

// Fonction             : __array_parser_count_size_of_dimensions
// Argument(s)          : - text : la chaine de caractères à parser
//			  - ndim : le nombre de dimensions (devrait être obtenu avec __array_parse_count_number_of_dimensions)
// Valeur de retour     : une tableau de taille ndim (alloué) contenant la taille de chaque dimension
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : compte la taille de chaque dimension
int *__array_parser_count_size_of_dimensions(char *text, int ndim)
{
	// allocation de la m�moire pour les dimensions
	int *dimsize = malloc(ndim * sizeof(int));
	if (dimsize == NULL)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_count_size_of_dimensions] error while allocating array\n");
		exit(EXIT_FAILURE);
	}

	// initialisation à 0 pour chaque dimension
	for (int i = 0; i < ndim; i++)
		dimsize[i] = 0;

	for (int i = 0; i < ndim; i++)
		__array_parser_depth_block_count(text, i + 1, dimsize);

	return dimsize;
}

// Fonction             : array_parser_parse_text
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : un pointeur sur une structure stenc_array remplit avec les infos issues du parsing
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : tente de parser toutes la valeurs dans la structure stenc_array
struct stenc_array *array_parser_parse_text(char *text)
{
	// vérification des accolades
	if (__array_parser_check_braces(text) == false)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_parse_text] Braces checking function __array_parser_check_braces returned false\n");
		exit(EXIT_FAILURE);
	}

	// vérification des virgules
	if (__array_parser_check_commas(text) == false)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_parse_text] Commas checking function __array_parser_check_commas returned false\n");
		exit(EXIT_FAILURE);
	}

	// extraction du nom
	char *id = __array_parser_extract_reference_identifier(text);

	// comptage du nombre de valeurs
	int nval = __array_parser_count_values(text);

	// comptage du nombre de dimensions
	int ndim = __array_parser_count_number_of_dimensions(text);

	// allocation de la mémoire pour la structure int_array
	struct stenc_array *arr = malloc(sizeof(struct stenc_array));
	// et ses données
	arr->identifier = id;
	arr->index_of_dimensions = NULL;
	arr->data = malloc(nval * sizeof(int));
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

////////////////////////////
// GESTION DES REFERENCES //
////////////////////////////

// Fonction             : __array_parser_extract_reference_identifier
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : une chaine de caractères contenant l'identificateur du tableau/de le référence
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : retourne l'identificateur de tableau
char *__array_parser_extract_reference_identifier(char *text)
{
	int i = 0;
	int id_begin = 0;
	int id_end = 0;

	while (i < strlen(text) + 1)
	{
		if ((text[i] >= 'A' && (text[i] <= 'Z') || ( text[i] >= 'a' && text[i] <= 'z') || text[i] == '_'))
		{
			id_begin = i;
			while (i < strlen(text) + 1 && ((text[i] >= 'A' && text[i] <= 'Z') || ( text[i] >= 'a' && text[i] <= 'z') || text[i] == '_'))
				i++;
			// extraction
			id_end = i;
			char *id = malloc((id_end - id_begin + 1) * sizeof(char));
			if (id == NULL)
			{
				fprintf(stderr, "[ARRAY_PARSER::__array_parser_extract_reference_identifier] ERROR while allocating memory for id\n");
				exit(EXIT_FAILURE);
			}
			strncpy(id, text + id_begin, (id_end - id_begin));
			id[id_end - id_begin] = '\0';
			return id;
		}
		i++;
	}
	return NULL;
}

// Fonction             : __array_parser_count_reference_dimensions
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : le nombre de références (d'indices) de la chaine
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : compte le nombre de dimensions de la référence (nombre de [])
int __array_parser_count_reference_dimensions(char *text)
{
	int i = 0;
	int opening_brackets = 0;
	int closing_brackets = 0;

	// parcours de l'id
	while (i < strlen(text) + 1 && ((text[i] >= 'A' && text[i] <= 'Z') || ( text[i] >= 'a' && text[i] <= 'z') || text[i] == '_'))
		i++;

	// parcours des références
	while (i < strlen(text) + 1)
	{
		if (text[i] == '[')
			opening_brackets++;
		if (text[i] == ']')
			closing_brackets++;
		i++;
	}

	if (opening_brackets == closing_brackets)
		return opening_brackets;
	else
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_extract_reference_identifier] ERROR while allocating memory for id\n");
		exit(EXIT_FAILURE);
	}
}

// Fonction             : __array_parser_count_index_of_dimensions
// Argument(s)          : - text : la chaine de caractères à parser
//			  - ndim : la nombre de dimensions de l'array (decrait être obtenu avec __array_parser_count_reference_dimensions)
// Valeur de retour     : une structure stenc_index de taille ndim (alloué) contenant l'indice pour chaque dimension (ou l'id)
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : remplit l'index de chaque dimension
struct stenc_index *__array_parser_count_index_of_dimensions(char *text, int ndim)
{
	// allocation de la mémoire pour le tableau des indices
	struct stenc_index *index_array = malloc(ndim * sizeof(struct stenc_index));
	if (index_array == NULL)
	{
		fprintf(stderr, "[ARRAY_PARSER::__array_parser_count_index_of_dimensions] ERROR while allocating memory for index_array\n");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	int index_begin = 0;
	int index_end = 0;
	int index_number = 0;

	// parcours de l'id
	while (i < strlen(text) + 1 && ((text[i] >= 'A' && text[i] <= 'Z') || ( text[i] >= 'a' && text[i] <= 'z') || text[i] == '_'))
		i++;

	// parcours des références
	while (i < strlen(text) + 1)
	{
		// l'indice est un nombre entier
		if (text[i] >= '0' && text[i] <= '9' || text[i] == '-')
		{
			index_begin = i;
			while (i < strlen(text) + 1 && (text[i] >= '0' && text[i] <= '9'))
				i++;
			// extraction de l'entier
			index_end = i;
			char *index = malloc((index_end - index_begin + 1) * sizeof(char));
			if (index == NULL)
			{
				fprintf(stderr, "[ARRAY_PARSER::__array_parser_count_index_of_dimensions] ERROR while allocating memory for index (int)\n");
				exit(EXIT_FAILURE);
			}
			strncpy(index, text + index_begin, (index_end - index_begin));
			index[index_end - index_begin] = '\0';
			index_array[index_number].is_identifier = false;
			index_array[index_number].value = atoi(index);
			//free(index);
			index_number++;
		}
		// l'indice est un identifier
		else if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z') || text[i] == '_')
		{
			index_begin = i;
			while (i < strlen(text) + 1 && (text[i] >= 'A' && text[i] <= 'Z') || 
			(text[i] >= 'a' && text[i] <= 'z') || (text[i] >= '0' && text[i] <= '9') || text[i] == '_')
				i++;
			// extraction de l'id
			index_end = i;
			char *index = malloc((index_end - index_begin + 1) * sizeof(char));
			if (index == NULL)
			{
				fprintf(stderr, "[ARRAY_PARSER::__array_parser_count_index_of_dimensions] ERROR while allocating memory for index (id)\n");
				exit(EXIT_FAILURE);
			}
			strncpy(index, text + index_begin, (index_end - index_begin));
			index[index_end - index_begin] = '\0';
			index_array[index_number].is_identifier = true;
			index_array[index_number].identifier = index;
			index_number++;
		}
		i++;
	}
	return index_array;
}

// Fonction             : array_parser_parse_reference
// Argument(s)          : - text : la chaine de caractères à parser
// Valeur de retour     : un pointeur sur une structure stenc_array remplit avec les infos issues du parsing de la chaine
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : parse toute la référence dans une structure stenc_array
struct stenc_array *array_parser_parse_reference(char *text)
{
	// extraction du nom
	char *id = __array_parser_extract_reference_identifier(text);

	// recherche du nombre dimensions
	int ndim = __array_parser_count_reference_dimensions(text);
	
	// remplissage de l'index de chaque dimension
	struct stenc_index *index_array = __array_parser_count_index_of_dimensions(text, ndim);

	// allocation de la structure
	struct stenc_array *arr = malloc(sizeof(struct stenc_array));
	if (arr == NULL)
	{
		fprintf(stderr, "[ARRAY_PARSER::array_parser_parse_reference] ERROR while allocating memory for struct int_array_reference\n");
		exit(EXIT_FAILURE);
	}

	// remplissage de la structure
	arr->identifier = id;
	arr->number_of_dimensions = ndim;
	arr->index_of_dimensions = index_array;
	arr->size_of_dimensions = NULL;	// tableaux uniquement
	arr->size_of_data = 0;	// à remplir

	return arr;
}

// Fonction             : array_parser_free
// Argument(s)          : - arr : un pointeur sur la structure stenc_array à libérer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : libère la mémoire allouée par la structure
void array_parser_free(struct stenc_array *arr)
{
	if (arr->identifier != NULL)
		free(arr->identifier);
	if (arr->size_of_dimensions != NULL)
		free(arr->size_of_dimensions);
	if (arr->data != NULL)
		free(arr->data);
	if (arr->index_of_dimensions != NULL)
	{
		if(arr->index_of_dimensions->is_identifier && arr->index_of_dimensions->identifier != NULL)
			free(arr->index_of_dimensions->identifier);
		free(arr->index_of_dimensions);
	}
	free(arr);
}

// Fonction             : array_parser_print
// Argument(s)          : - arr : un pointeur sur une structure stenc_array à afficher
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : affiche les champs de la structure stenc_array
void array_parser_print(struct stenc_array *arr)
{
	printf("STENC_STRUCT\n");
	printf("Identifier id %s\n", arr->identifier);
	printf("Number of dimensions is %d\n", arr->number_of_dimensions);
	printf("Number of values is %d\n", arr->size_of_data);
	printf("Size of each dimension :\n");
	if (arr->size_of_dimensions != NULL)
	{
		for (int i = 0; i < arr->number_of_dimensions; i++)
			printf("dimension %d is of size %d\n", i, arr->size_of_dimensions[i]);
	}
	printf("Index of each dimension (if reference) :\n");
	if (arr->index_of_dimensions != NULL)
	{
		for (int i = 0; i < arr->number_of_dimensions; i++)
		{
			if (arr->index_of_dimensions[i].is_identifier)
				printf("index of dimension %d is value of id %s\n", i, arr->index_of_dimensions[i].identifier);
			else
				printf("index on dimension %d is %d\n", i, arr->index_of_dimensions[i].value);
		}
	}
	printf("Data :\n");
	if (arr->data != NULL)
	{
		for (int i = 0; i < arr->size_of_data; i++)
			printf("data at index %d is %d\n", i, arr->data[i]);
	}
}
