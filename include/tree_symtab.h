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

/*=======================================================================*/
/* SECCIÓN DE IMPLEMENTACIÓN (ESTILO STB)                                 */
/*=======================================================================*/
#ifdef TREE_SYMTAB_IMPLEMENTATION
#ifndef TREE_SYMTAB_IMPLEMENTED_GUARD
#define TREE_SYMTAB_IMPLEMENTED_GUARD


// Por ahora mantenemos la tabla global estática original para no romper la compilación
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
#endif 
#endif