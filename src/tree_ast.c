#include "tree_ast.h"
#include <stdlib.h>

extern Arena astArena;

// Todas van SIN static porque Bison las necesita invocar al reducir reglas
struct ast* newast(int nodoValue, struct ast* left, struct ast* right, tData data) {
    struct ast* nuevo = arenaAlloc(&astArena, sizeof(struct ast));
    if (!nuevo) {
        tree_notify(ERR_SYS_NO_MEM_AST, "Fallo crítico en malloc de newast");
        return NULL;
    }
    nuevo->nodetype = nodoValue;
    nuevo->l = left;
    nuevo->r = right;
    nuevo->d = data;
    return nuevo;
}

struct ast* newflow(int nodetype, struct ast* cond, struct ast* iterable, struct ast* tblock, struct ast* fblock, struct symbol* s) {
    struct flow* a = arenaAlloc(&astArena, sizeof(struct flow));
    if (!a) {
        tree_notify(ERR_SYS_NO_MEM_FLOW, "Fallo crítico en malloc de newflow");
        return NULL;
    }
    a->nodetype = nodetype;
    a->cond = cond;
    a->iterable = iterable;
    a->tblock = tblock;
    a->fblock = fblock;
    a->s = s;
    return (struct ast*)a;
}

struct ast* newmemory_ast(int nodetype, struct symbol* s, struct ast* a) {
    struct memory_ast* nvo = arenaAlloc(&astArena, sizeof(struct memory_ast));
    if (!nvo) {
        tree_notify(ERR_SYS_NO_MEM_MEMORY, "Fallo crítico en malloc de newmemory_ast");
        return NULL;
    }
    nvo->nodetype = nodetype;
    nvo->s = s;
    nvo->a = a;
    return (struct ast*)nvo;
}

int get_nodetype(struct ast* a) {
    if (!a) {
        tree_notify(ERR_SYS_NULL_NODETYPE, "Se intentó leer el tipo de un nodo AST nulo.");
        return 0;
    }
    return a->nodetype;
}

int get_widht(struct ast* a) {
    if (!a) return 0;
    int widht = 0;
    while (get_nodetype(a) == NODE_LIST_OF_AST) {
        widht++;
        a = a->r;
    }
    widht++;
    return widht;
}