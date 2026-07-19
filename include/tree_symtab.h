#ifndef TREE_SYMTAB_H
#define TREE_SYMTAB_H

#include "../data.h"
#include "tree_nodetypes.h"
#include "tree_errors.h"
#include <../string.h>

#define NHASH 9997

// Forward declaration de ast para el body de las funciones
struct ast;

/*=======================================================================*/
/* ESTRUCTURAS DE SÍMBOLOS Y ENTORNOS                                    */
/*=======================================================================*/

struct symbol {
    char* name;
    tData data;
    struct symlist* args;
    struct ast* body;
};

struct symlist {
    struct symbol* s;
    struct symlist* next;
};

// Dejamos planteado el Environment para los Scopes Locales del Punto 5
struct env {
    struct symbol symbols[NHASH];
    struct env* outer; // Puntero al entorno padre (Scope superior)
};

/*=======================================================================*/
/* PROTOTIPOS DE LA TABLA DE SÍMBOLOS                                    */
/*=======================================================================*/

struct symbol* lookup(char* sym);
void add_definition(struct symbol* s, struct symlist* sl, struct ast* body);

struct symlist* addsym(struct symbol* s, struct symlist* sl);
void free_symlist(struct symlist* sl);
int compute_size(struct symlist* sl);

#endif

