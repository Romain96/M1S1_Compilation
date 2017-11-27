#ifndef __LIST_H__
#define __LIST_H__

#include "symbol_table.h"
#include "quad.h"

// structure de manipulation des listes de quads (pour les truelist/falselist)
struct list
{
        struct quad *current_quad;      // pointe sur un seul quad
        struct list *next;              // élément suivant dans la liste
};

// fonctions de manipulation des listes
struct list *list_new(struct quad *);
struct list *list_concat(struct list *, struct list *);
void list_free(struct list **);

#endif
