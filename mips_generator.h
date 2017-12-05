#ifndef __MIPS_GENERATOR_H__
#define __MIPS_GENERATOR_H__

#include <stdio.h>
#include "symbol_table.h"
#include "quad.h"

#define MIPS_MAX_LINE_SIZE 256

struct mips_generator
{
        FILE *output_file;
        struct symbol *symbol_table;
        struct quad *quad_list;
};

// Les fonctions décrites ici permettent de transformer la liste de quads en code assembleur MIPS
struct mips_generator *mips_setup(char *, struct symbol *, struct quad *);
void mips_generate_assembly_code(struct mips_generator *);
void mips_terminate(struct mips_generator *);

// écriture de la table des symboles en segment .data
void __mips_write_symbol_table(struct mips_generator *);
// écriture des instructions en segment .text
void __mips_write_quad_list(struct mips_generator *);

// génération d'une affectation
void __mips_generate_assignment(struct mips_generator *, struct quad *);

// fonctions de génération d'expressions arithmétiques
void __mips_generate_addition(struct mips_generator *, struct quad *);
void __mips_generate_substraction(struct mips_generator *, struct quad*);
void __mips_generate_multiplication(struct mips_generator *, struct quad *);
void __mips_generate_division(struct mips_generator *, struct quad *);

// génération d'un affichage d'une variable d'entier ou d'un entier
void __mips_generate_print_integer(struct mips_generator *, struct quad *);
// génération d'un affichage d'une chaîne de caractères
void __mips_generate_print_string(struct mips_generator *, struct quad *);

// génération des goto (conditionnels et inconditionnels)
void __mips_generate_goto(struct mips_generator *, struct quad *);

// fonctions de génération des conditions booléennes et des structures de contrôle
void __mips_generate_boolean_eq(struct mips_generator *, struct quad *);
void __mips_generate_boolean_ne(struct mips_generator *, struct quad *);
void __mips_generate_boolean_gt(struct mips_generator *, struct quad *);
void __mips_generate_boolean_lt(struct mips_generator *, struct quad *);
void __mips_generate_boolean_ge(struct mips_generator *, struct quad *);
void __mips_generate_boolean_le(struct mips_generator *, struct quad *);

#endif