#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
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
        // écriture de la table des symboles en segment .data
        __mips_write_symbol_table(mips);

        // écriture de la liste des quads en segment .text
        __mips_write_quad_list(mips);

        // terminaison de la génération du code assembleur
        mips_terminate(mips);
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
}

// Fonction             : __mips_write_symbol_table
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : écriture de la table des symboles en segment .data
void __mips_write_symbol_table(struct mips_generator *mips)
{
        // génération du segment .data
        char data_segment[] = ".data\n";
        fwrite(&data_segment, sizeof(char), strlen(data_segment) + 1, mips->output_file);

        // parcours de la table des symboles
        struct symbol *iterator = mips->symbol_table;
        while(iterator != NULL)
        {                   
                char line_to_write[MIPS_MAX_LINE_SIZE];
                snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "%s: .word %d\n", iterator->identifier, iterator->value);
                fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

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
        // génération du segent .text
        char text_segment[] = ".text\n";
        fwrite(&text_segment, sizeof(char), strlen(text_segment) + 1, mips->output_file);

        // TODO prendre en compte le main mieux que ça...
        char main_routine[] = "main:\n";
        fwrite(&main_routine, sizeof(char), strlen(main_routine) + 1, mips->output_file);

        // parcours de la liste des quads
        struct quad *iterator = mips->quad_list;
        while (iterator != NULL)
        {
                if (iterator->contain_goto == true)
                {
                        // TODO génération d'un goto
                        printf("goto pas encore pris en compte :(\n");
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
                                case QUAD_EQ:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
                                        break;
                                case QUAD_NE:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
                                        break;
                                case QUAD_GE:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
                                        break;
                                case QUAD_GT:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
                                        break;
                                case QUAD_LE:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
                                        break;
                                case QUAD_LT:
                                        // TODO booléens
                                        printf("booléens pas encore pris en compte :(\n");
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
        char prgramm_end[] = "li $v0, 10\nsyscall\n";
        fwrite(&prgramm_end, sizeof(char), strlen(prgramm_end) + 1, mips->output_file);

        printf("quad list written successfully :)\n");
}

// Fonction             : __mips_generate_assignment
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une affectation
void __mips_generate_assignment(struct mips_generator *mips, struct quad *q)
{
        // TODO assignment
        printf("on verra plus tard pour les affectation...\n");
        return;
}

// Fonction             : __mips_generate_addition
// Argument(s)          : - mips : une structure mips_generator générée par mips_setup
//                        - quad : le quad contenant la repésentation de l'instruction à générer
// Valeur de retour     : /
// Pré-condition(s)     : /
// Post-condition(s)    : /
// Commentaire(s)       : génère le code MIPS d'une addition
void __mips_generate_addition(struct mips_generator *mips, struct quad *q)
{
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, op1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, op2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 3) calculer la somme et placer le résultat dans un registre temporaire (add $t0, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "add $t0, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 4) placer la valeur de la somme du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);
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
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, op1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, op2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 3) calculer la différence et placer le résultat dans un registre temporaire (sub $t0, $t1, $t2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sub $t0, $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 4) placer la valeur de la fidérence du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);            
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
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, op1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, op2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 3) calculer la multiplication (mult $t1, $t2)
        // (Hi,Lo) est le résultat (on ne s'intéresse qu'à Hi pour la valeur entière)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mult $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 4) placer le résultat du registre Hi dans le registre temporaire $t0 (mfhi $t0)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mfhi $t0\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 5) placer la valeur de la somme du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);
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
        // 1) charger la valeur de l'opérande 1 dans le registre t1 (lw $t1, op1)
        char line_to_write[MIPS_MAX_LINE_SIZE];
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t1, %s\n", q->arg1->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 2) charger la valeur de l'opérande 2 dans le registre t2 (lw $t2, op2)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "lw $t2, %s\n", q->arg2->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 3) calculer la division (div $t1, $t2)
        // le quotient est stocké dans Lo et le reste Hi
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "div $t1, $t2\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 4) placer le résultat du registre Lo dans le registre temporaire $t0 (mflo $t0)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "mflo $t0\n");
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);

        // 5) placer la valeur de la somme du registre vers la ram (sw $t0, res)
        snprintf(line_to_write, MIPS_MAX_LINE_SIZE, "sw $t0, %s\n", q->res->identifier);
        fwrite(line_to_write, sizeof(char), strlen(line_to_write) + 1, mips->output_file);
}
