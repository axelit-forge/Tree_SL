#include "tree_symtab.h"
#include "tree_errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol symtab[NHASH];

static unsigned int symhash(char *sym) {
    unsigned int hash = 0;
    int c;
    while ((c = *sym)) {
        hash = (hash * 9) ^ c;
        sym++;
    }
    return hash;
}

struct symbol* lookup(char *sym) {
    if (!sym) tree_notify(ERR_SYS_NULL_POINTER, "en lookup: nombre de símbolo nulo");

    unsigned int index = symhash(sym) % NHASH;
    struct symbol* sp = &symtab[index];
    int count = 1;

    while (count <= NHASH) {
        if (!sp->name) { // Celda vacía, creamos el símbolo
            sp->name = strdup(sym);
            sp->data = NULL;
            sp->args = NULL;
            sp->body = NULL;
            return sp;
        }
        else if (strcmp(sp->name, sym) == 0) { // Encontrado
            return sp;
        }
        else {
            sp++;
            if (sp >= symtab + NHASH) {
                sp = symtab;
            }
            count++;
        }
    }

    tree_notify(ERR_SYS_OVERFLOW_SYMTAB, sym);
    return NULL;
}

void add_definition(struct symbol* s, struct symlist* sl, struct ast* body) {
    if (!s) tree_notify(ERR_SYS_NULL_POINTER, "en add_definition");

    if (s->args) {
        free_symlist(s->args);
    }
    // NOTA: Cuando implementemos la Arena, la liberación del body viejo
    // será automática al limpiar la arena.

    s->args = sl;
    s->body = body;
}

struct symlist* addsym(struct symbol* s, struct symlist* sl) {
    struct symlist* lista = malloc(sizeof(struct symlist));
    if (!lista) {
        tree_notify(ERR_SYS_NO_MEM_MEMORY, "No hay memoria para expandir symlist (addsym)");
        return NULL;
    }
    lista->s = s;
    lista->next = sl;
    return lista;
}

void free_symlist(struct symlist* sl) {
    if (!sl) return;
    struct symlist* next = sl->next;
    free(sl);
    free_symlist(next);
}

int compute_size(struct symlist* sl) {
    int size = 0;
    while (sl) {
        size++;
        sl = sl->next;
    }
    return size;
}