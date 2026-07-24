#include "tree_symtab.h"

extern Arena astArena;
static Scope *current_scope = NULL;

/*=======================================================================*/
/* FUNCIÓN DE HASH INTERNA                                        */
/*=======================================================================*/

static unsigned int symhash(const char *sym) {
    unsigned int hash = 0;
    int c;
    while ((c = *sym)) {
        hash = (hash * 9) ^ c;
        sym++;
    }
    return hash;
}
/*=======================================================================*/
/* GESTIÓN DE ÁMBITOS (SCOPES)                                          */
/*=======================================================================*/

void init_symtab(void) {
    current_scope = arenaAlloc(&astArena, sizeof(Scope));
    if (!current_scope) {
        tree_notify(ERR_SYS_NO_MEM_MEMORY, "No hay memoria para inicializar el ámbito global");
        return;
    }

    memset(current_scope->symtab, 0, sizeof(struct symbol) * NHASH);
    current_scope->parent = NULL;
}

void push_scope(void) {
    Scope *new_scope = arenaAlloc(&astArena, sizeof(Scope));
    if (!new_scope) {
        tree_notify(ERR_SYS_NO_MEM_MEMORY, "No hay memoria para crear un nuevo ámbito (push_scope)");
        return;
    }

    memset(new_scope->symtab, 0, sizeof(struct symbol) * NHASH);
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void pop_scope() {
    if (current_scope && current_scope->parent) {
        current_scope = current_scope->parent;
    } else {
        tree_notify(ERR_SYS_NULL_POINTER, "Intento de desapilar el ámbito global (pop_scope)");
    }
}

/*=======================================================================*/
/* BÚSQUEDA E INSERCIÓN DE SÍMBOLOS                                     */
/*=======================================================================*/


struct symbol* lookup_local(char *sym) {
    if (!sym) {
        tree_notify(ERR_SYS_NULL_POINTER, "en lookup_local: nombre de símbolo nulo");
        return NULL;
    }
    if (!current_scope) return NULL;

    unsigned int index = symhash(sym) % NHASH;
    struct symbol *sp = &current_scope->symtab[index];
    int count = 1;

    while (count <= NHASH) {
        if (!sp->name) {
            return NULL;
        }
        if (strcmp(sp->name, sym) == 0) {
            return sp;
        }
        sp++;
        if (sp >= current_scope->symtab + NHASH) {
            sp = current_scope->symtab;
        }
        count++;
    }

    return NULL;
}

struct symbol* insert_local(char *sym) {
    if (!sym) {
        tree_notify(ERR_SYS_NULL_POINTER, "en insert_local: nombre de símbolo nulo");
        return NULL;
    }
    if (!current_scope) {
        tree_notify(ERR_SYS_NULL_POINTER, "en insert_local: current_scope no inicializado");
        return NULL;
    }

    unsigned int index = symhash(sym) % NHASH;
    struct symbol *sp = &current_scope->symtab[index];
    int count = 1;

    while (count <= NHASH) {
        if (!sp->name) {
            sp->name = strdup(sym);
            sp->data = NULL;
            sp->args = NULL;
            sp->body = NULL;
            return sp;
        }
        if (strcmp(sp->name, sym) == 0) {
            return sp;
        }
        sp++;
        if (sp >= current_scope->symtab + NHASH) {
            sp = current_scope->symtab;
        }
        count++;
    }

    tree_notify(ERR_SYS_OVERFLOW_SYMTAB, sym);
    return NULL;
}

struct symbol* lookup(char *sym) {
    if (!sym) {
        tree_notify(ERR_SYS_NULL_POINTER, "en lookup: nombre de símbolo nulo");
        return NULL;
    }

    Scope *scope_iter = current_scope;

    while (scope_iter != NULL) {
        unsigned int index = symhash(sym) % NHASH;
        struct symbol *sp = &scope_iter->symtab[index];
        int count = 1;

        while (count <= NHASH) {
            if (!sp->name) {
                break;
            }
            if (strcmp(sp->name, sym) == 0) {
                return sp;
            }
            sp++;
            if (sp >= scope_iter->symtab + NHASH) {
                sp = scope_iter->symtab;
            }
            count++;
        }
        scope_iter = scope_iter->parent;
    }

    return insert_local(sym);
}

/*=======================================================================*/
/* FUNCIONES AUXILIARES Y MANEJO DE LISTAS DE SÍMBOLOS                  */
/*=======================================================================*/

void add_definition(struct symbol* s, struct symlist* sl, struct ast* body) {
    if (!s) {
        tree_notify(ERR_SYS_NULL_POINTER, "en add_definition: símbolo nulo");
        return;
    }

    s->args = sl;
    s->body = body;
}

struct symlist* addsym(struct symbol* s, struct symlist* sl) {
    struct symlist* lista = arenaAlloc(&astArena, sizeof(struct symlist));
    if (!lista) {
        tree_notify(ERR_SYS_NO_MEM_MEMORY, "No hay memoria para expandir symlist (addsym)");
        return NULL;
    }
    lista->s = s;
    lista->next = sl;
    return lista;
}

void free_symlist(struct symlist* sl) {
    (void)sl;
}

int compute_size(struct symlist* sl) {
    int size = 0;
    while (sl) {
        size++;
        sl = sl->next;
    }
    return size;
}