#include <stdlib.h>
#include "symbol_table.h"
#include "quad.h"
#include "list.h"

// crée une nouvelle list de pointeurs de quads
// ne contient pas les quads eux-mêmes mais les pointeurs vers les quads
// qui sont dans une struct quad *
struct list *list_new(struct quad *q)
{
        // allocation de la liste
        struct list *new = malloc(sizeof(struct list));
        new->current_quad = q;
        new->next = NULL;
        return new;
}

// concatène les listes de quads l1 et l2 et retourne une nouvelle liste de quads
struct list *list_concat(struct list *l1, struct list *l2)
{
        // si les deux listes sont vide, on retourne NULL
        if (l1 == NULL && l2 == NULL)
                return NULL;

        // si l1 est vide on retourne l2
        if (l1 == NULL)
                return l2;

        // si l2 est vide on retourne l1
        if (l2 == NULL)
                return l1;

        // sinon on parcours l1 jusqu'à la fin et on fait pointer le dernier element->next sur l2
        struct list *iterator = l1;
        while (iterator->next != NULL)
                iterator = iterator->next;
        iterator->next = l2;
        // et on retourne l1 qui désormais contient l1 et l2
        return l1;
}