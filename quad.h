#ifndef __QUAD_H__
#define __QUAD_H__

#include "symbol_table.h"

// définit tous les opérateurs possibles dans un quad
enum quad_operator{ QUAD_PLUS, QUAD_MINUS, QUAD_MULTIPLY, QUAD_DIVIDE, QUAD_EQ, QUAD_NE, QUAD_GT, QUAD_LT, QUAD_GE, QUAD_LE, QUAD_ASSIGNMENT, QUAD_GOTO};

// structure de quad définie comme une liste chainée de quads
struct quad
{
	int id;			// id du quad (ie son numéo)
	enum quad_operator op;	// operator
	struct symbol *arg1;	// argument 1
	struct symbol *arg2;	// argument 2
	struct symbol *res;	// resultat
	struct quad *next;
};

// fonctions de manipulation des (listes de) quads
struct quad *quad_gen(struct quad **, enum quad_operator, struct symbol *, struct symbol *, struct symbol *);
struct quad *quad_add(struct quad *, struct quad *);
void quad_print(struct quad *);

#endif
