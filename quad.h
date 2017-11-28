#ifndef __QUAD_H__
#define __QUAD_H__

#include "symbol_table.h"

// définit tous les opérateurs possibles dans un quad
enum quad_operator{ QUAD_NO_OP, QUAD_PLUS, QUAD_MINUS, QUAD_MULTIPLY, QUAD_DIVIDE, QUAD_EQ, QUAD_NE, QUAD_GT, QUAD_LT, QUAD_GE, QUAD_LE, QUAD_ASSIGNMENT, QUAD_INCREASE, QUAD_DECREASE, QUAD_AND, QUAD_OR, QUAD_NOT, QUAD_PRINTF, QUAD_PRINTI};

// structure de quad définie comme une liste chainée de quads
struct quad
{
	int id;			// id du quad (ie son numéro)
	enum quad_operator op;	// operator
	struct symbol *arg1;	// argument 1
	struct symbol *arg2;	// argument 2
	struct symbol *res;	// resultat
	bool contain_goto;	// indique si l'instruction contient un goto
	int goto_quad;		// si l'instruction est "if condition goto ?" contient le goto à remplir
	struct quad *next;
};

// fonctions de manipulation des (listes de) quads
struct quad *quad_gen(struct quad **, enum quad_operator, struct symbol *, struct symbol *, struct symbol *, bool, int);
struct quad *quad_concat(struct quad *, struct quad *);
void quad_free(struct quad **);
void quad_print(struct quad *);

#endif
