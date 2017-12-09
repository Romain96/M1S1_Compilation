#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "quad.h"
#include "list.h"
#include "mips_generator.h"

// Fonction             : mips_setup
// Argument(s)          : - filename : le nom du fichier dans lequel écrire le code MIPS
//                        - symbol_table : la table des symboles complétée par l'anlyse lexicale/syntaxique
//                        - quad_list : la liste des quads produits lors de l'analyse syntaxique
// Valeur de retour     : une structure mips_generator
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : /
struct mips_generator *mips_setup(char *filename, struct symbol *symbol_table, struct quad *quad_list)
{
        // ouverture en écriture, création si non existant, troncature si existant
        FILE *output = fopen(filename, "w+");
        if (output == NULL)
        {
                perror("mips_generator::mips_setup : error while opening the output file\n");
                exit(EXIT_FAILURE);
        }

        // allocation de la structure mips_generator
        struct mips_generator *mips = malloc(sizeof(struct mips_generator));
        if (mips == NULL)
        {
                fprintf(stderr, "mips_generator : error while allocating space for mips_generator struct\n");
                exit(EXIT_FAILURE);
        } 

        // remplissage de la structure
        mips->output_file = output;
        mips->symbol_table = symbol_table;
        mips->quad_list = quad_list;

        return mips;
}

// Fonction             : mips_generate_assembly_code
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : écrit tout le code assembleur
void mips_generate_assembly_code(struct mips_generator *mips)
{
	printf("\n//////////////////////////////////////////////////\n");
	printf("///////// Writting MIPS assembly code... /////////\n");
	printf("//////////////////////////////////////////////////\n");
        // écriture de la table des symboles en segment .data
        __mips_write_symbol_table(mips);

        // écriture de la liste des quads en segment .text
        __mips_write_quad_list(mips);

        // terminaison de la génération du code assembleur
        mips_terminate(mips);
	printf("MIPS assembly code generated successfully\n");
}

// Fonction             : mips_terminate
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : ferme le fichier d'écriture et libère la mémoire allouée pour la structure mips_generator
void mips_terminate(struct mips_generator *mips)
{
        // fermeture du fichier de sortie
        if (fclose(mips->output_file) != 0)
        {
                perror("mips_generator::mips_terminate : error while closing the output file\n");
                exit(EXIT_FAILURE);
        }

        // désallocation de la table des symboles
        symbol_free(&mips->symbol_table);

        // désallocation de la liste des quads
        quad_free(&mips->quad_list);

        // désallocation de la mémoire de la structure mips_generator
	free(mips);
}

//===================================================================================
// FONCTIONS D'ECRITURE : ECRITURE DE LA TABLE DES SYMBOLES ET DE LA LISTE DE QUADS
//===================================================================================

// Fonction             : __mips_write_symbol_table
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : écriture de la table des symboles en segment .data
void __mips_write_symbol_table(struct mips_generator *mips)
{
	printf("Writting symbol table...\n");
        // génération du segment .data
        char data_segment[] = ".data\n";
        fwrite(&data_segment, sizeof(char), strlen(data_segment), mips->output_file);

        // parcours de la table des symboles
        struct symbol *iterator = mips->symbol_table;
        while(iterator != NULL)
        {                   
                char line_to_write[MIPS_MAX_LINE_SIZE];

		if (iterator->is_string_litteral)
		// variable chaine de caractères
		{
			snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "%s: .asciiz %s\n", iterator->identifier, iterator->string_value);
			fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
		}
                // variable tableau
                else if (iterator->is_int_array)
                {
                        char value[] = "0, ";
                        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "%s: .word ", iterator->identifier);
                        for (int i = 0; i < iterator->int_array_value->size_of_data - 1; i++)
                        {
                                strncat(line_to_write, value, strlen(value));
                        }
                        strncat(line_to_write, "0\n", 2);
                        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
                }
                // variable entière
                else
		{
                	snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "%s: .word %d\n", iterator->identifier, iterator->int_value);
                	fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
		}
                iterator = iterator->next;
        }
        printf("Symbol table written successfully :)\n");
}

// Fonction             : __mips_write_quad_list
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : écriture des instructions en segment .text
void __mips_write_quad_list(struct mips_generator *mips)
{
	printf("Writting quad list...\n");
        // génération du segent .text
        char text_segment[] = ".text\n";
        fwrite(&text_segment, sizeof(char), strlen(text_segment), mips->output_file);

        // définir main en tant que symbole global
        char main_global[] = ".globl main\n";
        fwrite(&main_global, sizeof(char), strlen(main_global), mips->output_file);

        // TODO prendre en compte le main mieux que ça...
        char main_routine[] = "main:\n";
        fwrite(&main_routine, sizeof(char), strlen(main_routine), mips->output_file);

        // parcours de la liste des quads
        struct quad *iterator = mips->quad_list;
        while (iterator != NULL)
        {
                // Avant d'écrire les instructions il faut écrire le nom du label si le quad en a un
                if (iterator->is_labelled)
                {
                        char line_to_write[MIPS_MAX_LINE_SIZE];
                        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "%s: ", iterator->label_name);
                        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
                }

                // si le quad contient un goto il est traité séparément
                if (iterator->contain_goto == true)
                {
                        // génère un goto (in)conditionnel et éventuellement une comparaison booléenne
                        __mips_generate_goto(mips, iterator);
                }
                else
                {
                        // ne contient pas de goto
                        switch (iterator->op)
                        {
                                case QUAD_PLUS:
                                        __mips_generate_addition(mips, iterator);
                                        break;
                                case QUAD_MINUS:
                                        __mips_generate_substraction(mips, iterator);
                                        break;
                                case QUAD_MULTIPLY:
                                        __mips_generate_multiplication(mips, iterator);
                                        break;
                                case QUAD_DIVIDE:
                                        __mips_generate_division(mips, iterator);
                                        break;
                                case QUAD_ASSIGNMENT:
                                        __mips_generate_assignment(mips, iterator);
                                        break;
                                case QUAD_PRINTI:
                                        __mips_generate_print_integer(mips, iterator);
                                        break;
                                case QUAD_PRINTF:
                                        __mips_generate_print_string(mips, iterator);
                                        break;
                                case QUAD_ARRAY_READ:
                                        __mips_generate_array_read(mips, iterator);
                                        break;
                                case QUAD_ARRAY_WRITE:
                                        __mips_generate_array_write(mips, iterator);
                                        break;
                                default:
                                        printf("cas normalement impossible !\n");
                                        break;
                        }
                }
                // quad suivant
                iterator = iterator->next;
        }

        // termine le programme en appellant le syscall de terminaison
        char prgramm_end[] = "label_end: li $v0, 10\nsyscall\n";
        fwrite(&prgramm_end, sizeof(char), strlen(prgramm_end), mips->output_file);

        printf("quad list written successfully :)\n");
}

//===============================================================================
// AFFECTATION D'UNE VALEUR A UN IDENTIFICATEUR
//===============================================================================

// Fonction             : __mips_generate_assignment
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une affectation
void __mips_generate_assignment(struct mips_generator *mips, struct quad *q)
{
	// 1) charger le temporaire contenant la valeur à assigner dans le registre $t1 (lw $t0, arg2)
	char line_to_write[MIPS_MAX_LINE_SIZE];
	snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t0, %s\n",  q->arg2->identifier);
	fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

	// 2) placer la valeur de $t0 dans la RAM à l'adresse contenant l'identificateur
	snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->arg1->identifier);
	fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

//===============================================================================
// FONCTIONS ARITHMETIQUES : ADDITION, SOUSTRACTION, MULTIPLICATION, DIVISION
//===============================================================================

// Fonction             : __mips_generate_addition
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une addition
void __mips_generate_addition(struct mips_generator *mips, struct quad *q)
{
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, arg2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) calculer la somme et placer le résultat dans un registre temporaire (add $t0, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "add $t0, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) placer la valeur de la somme du registre vers la ram (sw $t0, arg1)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_substraction
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une soustraction
void __mips_generate_substraction(struct mips_generator *mips, struct quad *q)
{
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, arg2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) calculer la différence et placer le résultat dans un registre temporaire (sub $t0, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sub $t0, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) placer la valeur de la fidérence du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);            
}

// Fonction             : __mips_generate_multiplication
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une multiplication
void __mips_generate_multiplication(struct mips_generator *mips, struct quad *q)
{
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, arg2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) calculer la multiplication (mult $t1, $t2)
        // (Hi,Lo) est le résultat (on ne s'intéresse qu'à Hi pour la valeur entière)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mult $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) placer le résultat du registre Hi dans le registre temporaire $t0 (mfhi $t0)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mfhi $t0\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 5) placer la valeur de la somme du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_division
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une division
void __mips_generate_division(struct mips_generator *mips, struct quad *q)
{
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, arg2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) calculer la division (div $t1, $t2)
        // le quotient est stocké dans Lo et le reste Hi
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "div $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) placer le résultat du registre Lo dans le registre temporaire $t0 (mflo $t0)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mflo $t0\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 5) placer la valeur de la somme du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

//========================================================================================
// FONCTIONS D'AFFICHAGE : AFFICHAGE D'UN ENTIER ET AFFICHAGE D'UNE CHAINE DE CARACTERES
//========================================================================================

// Fonction             : __mips_generate_print_integer
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS pour afficher un entier à l'écran (printi)
void __mips_generate_print_integer(struct mips_generator *mips, struct quad *q)
{
        // 1) placer la valeur de l'entier dans le registre $a0 (lw $a0, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $a0, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur 1 (print integer) dans $v0 (li $v0, 1)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "li $v0, 1\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // appeller syscall (syscall)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "syscall\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_print_string
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS pour afficher une chaine de caractères à l'écran
void __mips_generate_print_string(struct mips_generator *mips, struct quad *q)
{
        // 1) placer l'adresse de la chaine à afficher dans $a0 (la $a0, arg1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "la $a0, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur 4 (print string) dans $v0 (li $v0, 4)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "li $v0, 4\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3/ appel à syscall (syscall)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "syscall\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

//===================================================================
// FONCTION DE GENERATION DES GOTO CONDITIONNELS ET INCONDITIONNELS
//===================================================================

// Fonction             : __mips_generate_goto
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS des goto conditionnels et inconditionnels
void __mips_generate_goto(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];
        // si le quad est un goto inconditionnel, on génère juste de goto
        if (q->op == QUAD_NO_OP)
        {
                // génération du goto label (b label_name)
                snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "b %s\n", q->goto_quad);
                fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
        }
        // sinon on génère la comparaison nécessaire puis le goto
        else
        {
                switch (q->op)
                {
                        case QUAD_EQ:
                                __mips_generate_boolean_eq(mips, q);
                                break;
                        case QUAD_NE:
                                __mips_generate_boolean_ne(mips, q);
                                break;
                        case QUAD_GT:
                                __mips_generate_boolean_gt(mips, q);
                                break;
                        case QUAD_LT:
                                __mips_generate_boolean_lt(mips, q);
                                break;
                        case QUAD_GE:
                                __mips_generate_boolean_ge(mips, q);
                                break;
                        case QUAD_LE:
                                __mips_generate_boolean_le(mips, q);
                                break;
                        default:
                                fprintf(stderr, "quad is not supposed to be here...\n");
                                exit(EXIT_FAILURE);
                }
        }
}

//==================================================================================
// FONCTIONS DE GENERATION DES CONDITIONS BOOLEENNES ET DES STRUCTURES DE CONTROLE
//==================================================================================

// Fonction             : __mips_generate_boolean_eq
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type ==
void __mips_generate_boolean_eq(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bgt $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "beq $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_boolean_ne
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type !=
void __mips_generate_boolean_ne(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bne $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "bne $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_boolean_gt
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type >
void __mips_generate_boolean_gt(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bgt $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "bgt $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_boolean_lt
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type <
void __mips_generate_boolean_lt(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bgt $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "blt $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_boolean_ge
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type >=
void __mips_generate_boolean_ge(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bgt $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "bge $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_boolean_le
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une comparaison de type <=
void __mips_generate_boolean_le(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // 1) placer la valeur de arg1 dans le registre $t1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer la valeur de arg2 dans le registre $t2
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) génère un saut conditionnel (bgt $t1, $t2, label)   
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "ble $t1, $t2, %s\n", q->goto_quad);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

//==================================================================================
// FONCTIONS DE GENERATION DES ACCES AUX TABLEAUX
//==================================================================================

// Fonction             : __mips_generate_array_read
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'un accès à une référence de tableau
void __mips_generate_array_read(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];

        // ici le quad code l'opération arg1 = arg2[res]
        // avec arg1 la variable à affecter, res l'offset et arg2 le nom du tableau
        
        // 1) placer l'adresse de base du tableau dans le registre $t1 (la $t1, arg2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "la $t1, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer l'offset (déjà multiplié par NBO = 4) de l'élément à lire dans le registre $t2 (lw $t2, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) Additionner l'adresse de base avec l'offset et placer le résultat dans $t1 (add $t1, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "add $t1, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) charger la valeur lue à l'adresse $t1 dans $t0 (lw $t0, 0($t1))
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t0, 0($t1)\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 5) placer la valeur de $t0 dans arg1
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}

// Fonction             : __mips_generate_array_write
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une affectation de référence de tableau
void __mips_generate_array_write(struct mips_generator *mips, struct quad *q)
{
        char line_to_write[MIPS_MAX_LINE_SIZE];
        
        // ici le quad code l'opération arg1[res] = arg2
        // avec arg1 le nom du tableau, res l'offset et arg2 la variable à affecter

        // 1) placer l'adresse de base du tableau dans le registre $t1 (la $t1, arg1)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "la $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 2) placer l'offset (déjà multiplié par NBO = 4) de l'élément à lire dans le registre $t2 (lw $t2, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 3) Additionner l'adresse de base avec l'offset et placer le résultat dans $t1 (add $t1, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "add $t1, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);

        // 4) placer la valeur de arg2 dans $t0
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t0, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
 
        // 5) placer la valeur de $t0 dans le tableau de arg1 à l'adresse contenue dans $t1 (sw $t0 0($t1))
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, 0($t1)\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write), mips->output_file);
}