#ifndef TREE_AST_H
#define TREE_AST_H

#include "tree_data.h"
#include "tree_errors.h"
#include "tree_nodetypes.h"

// Forward declaration de symbol para no requerir la cabecera symtab aquí
struct symbol; 

struct ast {
    int nodetype;
    struct ast* l;
    struct ast* r;
    tData d;
};

struct flow {
    int nodetype;
    struct ast* cond;
    struct ast* iterable;
    struct ast* tblock;
    struct ast* fblock;
    struct symbol* s;
};

struct memory_ast {
    int nodetype;
    struct symbol* s;
    struct ast* a; 
};

struct ast* newast(int nodoValue, struct ast* left, struct ast* right, tData data);
struct ast* newflow(int nodetype, struct ast* cond, struct ast* iterable, struct ast* tblock, struct ast* fblock, struct symbol* s);
struct ast* newmemory_ast(int nodetype, struct symbol* s, struct ast* a);
int get_nodetype(struct ast* a);
int get_widht(struct ast* a);

#endif
