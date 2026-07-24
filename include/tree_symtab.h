#ifndef TREE_SYMTAB_H
#define TREE_SYMTAB_H

#include "tree_data.h"
#include "tree_nodetypes.h"
#include "tree_errors.h"
#include <string.h>

#define NHASH 9997

// Forward declaration de ast para el body de las funciones

/*=======================================================================*/
/* ESTRUCTURAS DE SÍMBOLOS Y ENTORNOS                                    */
/*=======================================================================*/

struct symbol {
    char* name;
    tData data;
    struct symlist* args;
    struct ast* body;
};

//struct symbol symtab[NHASH];

typedef struct Scope {
    struct symbol symtab[NHASH];
    struct Scope *parent;
} Scope;

struct symlist {
    struct symbol* s;
    struct symlist* next;
};

/*=======================================================================*/
/* PROTOTIPOS DE LA TABLA DE SÍMBOLOS                                    */
/*=======================================================================*/

void init_symtab();
void push_scope();
void pop_scope();

struct symbol* lookup(char* sym);
struct symbol* lookup_local(char *sym);
struct symbol* insert_local(char *sym);
void add_definition(struct symbol* s, struct symlist* sl, struct ast* body);

struct symlist* addsym(struct symbol* s, struct symlist* sl);
void free_symlist(struct symlist* sl);
int compute_size(struct symlist* sl);

#endif

