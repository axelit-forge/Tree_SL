#include "tree_eval.h"
#include "tree_ast.h"
#include "tree_symtab.h"
#include "tree_errors.h"
#include "tree_data.h"
#include <stdio.h>
#include <stdlib.h>

/*=======================================================================*/
/* 6. FUNCIONES DELEGADAS MÁS COHESIVAS Y LIMPIAS                        */
/*=======================================================================*/

static tData eval_aritmetica(int op, tData l, tData r) {
    if (!l || !r) {
        tree_notify(ERR_SYS_NULL_POINTER, "Operandos nulos pasados a eval_aritmetica");
    }

    int tl = get_tipo(l);
    int tr = get_tipo(r);

    if ((tl != NODE_INT && tl != NODE_DOUBLE) || (tr != NODE_INT && tr != NODE_DOUBLE)) {
        tree_notify(ERR_SEM_NUMBERS_ONLY, "Intentaste operar tipos no numéricos en una expresión matemática");
    }

    if (op == '/' && ((tr == NODE_INT && get_value(r) == 0) || (tr == NODE_DOUBLE && get_real(r) == 0.0))) {
        tree_notify(ERR_RUN_DIVISION_BY_ZERO, "Ocurrió en una operación de cociente (/)");
    }

    switch (op) {
        case '+': return sumaData(l, r);
        case '-': return restaData(l, r);
        case '*': return prodData(l, r);
        case '/': return cocData(l, r);
        default:  return NULL;
    }
}

static tData eval_relacional(int op, tData l, tData r) {
    if (!l || !r) tree_notify(ERR_SYS_NULL_POINTER, "en eval_relacional");

    if (get_tipo(l) != get_tipo(r)) {
        tree_notify(ERR_SEM_TYPE_MISMATCH, NULL);
    }

    switch (op) {
        case NODE_MAYOR:       return compara_mayor(l, r);
        case NODE_MENOR:       return compara_menor(l, r);
        case NODE_MAYOR_IGUAL: return compara_mayorigual(l, r);
        case NODE_MENOR_IGUAL: return compara_menorigual(l, r);
        case NODE_IGUAL:       return compara_igual(l, r);
        case NODE_DISTINTO:    return compara_distinto(l, r);
        default:          return NULL;
    }
}


static tData eval_unario(int op, tData l) {
    if (!l) {
        tree_notify(ERR_SYS_NULL_POINTER, "en eval_unario");
        return NULL;
    }

    switch (op) {
        case NODE_MENOS_UNARIO:
            return negarData(l);
        case NODE_MODULO:
            return moduloData(l);
        default:
            return NULL;
    }
}

static tData eval_logica(int op, struct ast *a) {
    if (!a) {
        tree_notify(ERR_SYS_NULL_POINTER, "Árbol nulo en eval_logica");
        return NULL;
    }

    struct ast *left = a->l;
    struct ast *right = a->r;

    if (op == NODE_NOT) {
        if (!right) {
            tree_notify(ERR_SYS_NULL_POINTER, "Operando derecho nulo en NOT");
            return NULL;
        }
        tData res_r = eval(right);
        return get_bool_value(res_r) ? createBool("false") : createBool("true");
    }

    if (!left || !right) {
        tree_notify(ERR_SYS_NULL_POINTER, "Faltan operandos en operación lógica binaria");
        return NULL;
    }

    tData res_l = eval(left);
    tData res_r = eval(right);

    switch (op) {
        case NODE_AND:
            if (get_bool_value(res_l) && get_bool_value(res_r)) {
                return createBool("true");
            }
            return createBool("false");

        case NODE_OR:
            if (get_bool_value(res_l) || get_bool_value(res_r)) {
                return createBool("true");
            }
            return createBool("false");

        default:
            return NULL;
    }
}

static tData eval_operacion_lista(int op, struct ast *a) {
    if (!a || !a->l || !a->r) {
        tree_notify(ERR_SYS_NULL_POINTER, "Operandos nulos en eval_operacion_lista");
        return NULL;
    }

    tData aux_result = eval(a->l);
    tData list_result = eval(a->r);

    if (!aux_result || !list_result) {
        tree_notify(ERR_SYS_NULL_POINTER, "Subexpresión evaluada como nula en lista");
        return NULL;
    }

    switch (op) {
        case NODE_KICK:   return kickData(aux_result, list_result);
        case NODE_ADD:    return addData(aux_result, list_result);
        case NODE_TAKE:   return takeData(aux_result, list_result);
        case NODE_CONCAT: return concatData(aux_result, list_result);
        case NODE_GET:    return getData(aux_result, list_result);
        default:
            tree_notify(ERR_RUN_GENERIC_EXECUTION, "Operación de lista no soportada");
            return NULL;
    }
}
static tData eval_operacion_conjunto(int op, struct ast *a) {
    if (!a || !a->l || !a->r) {
        tree_notify(ERR_SYS_NULL_POINTER, "Operandos nulos en eval_operacion_conjunto");
        return NULL;
    }

    tData conj_1 = eval(a->l);
    tData conj_2 = eval(a->r);

    if (!conj_1 || !conj_2) {
        tree_notify(ERR_SYS_NULL_POINTER, "Subexpresión evaluada como nula en conjunto");
        return NULL;
    }

    switch (op) {
        case NODE_IN:       return inData(conj_1, conj_2);
        case NODE_UNION:    return unionData(conj_1, conj_2);
        case NODE_DIFF:     return diffData(conj_1, conj_2);
        case NODE_INTER:    return interData(conj_1, conj_2);
        case NODE_CONTAINS: return containsData(conj_1, conj_2);
        default:
            tree_notify(ERR_RUN_GENERIC_EXECUTION, "Operación de conjunto no soportada");
            return NULL;
    }
}

static tData eval_flow(struct flow *a) {
    if (!a) {
        tree_notify(ERR_SYS_NULL_POINTER, "Estructura flow nula en eval_flow");
        return NULL;
    }

    struct ast *cond = a->cond;
    struct ast *iterable = a->iterable;
    struct ast *tblock = a->tblock;
    struct ast *fblock = a->fblock;
    struct symbol *s = a->s;

    tData nuevo = NULL;

    switch (get_nodetype((struct ast *)a)) {
        case NODE_IF: {
            if (get_bool_value(eval(cond))) {
                nuevo = eval(tblock);
            } else if (fblock != NULL) {
                nuevo = eval(fblock);
            }
            break;
        }
        case NODE_WHILE: {
            while (get_bool_value(eval(cond))) {
                nuevo = eval(tblock);
            }
            break;
        }
        case NODE_FORALL: {
            tData collection = copiarData(eval(iterable));
            tData iterator = collection;

            while (iterator) {
                tData elemento = get_dato(iterator);
                int booleano = cond ? get_bool_value(eval(cond)) : 1;

                if (booleano != 0) {
                    s->data = copiarData(elemento);
                    nuevo = eval(tblock);
                }
                iterator = get_next(iterator);
            }
            freeData(collection);
            break;
        }
        case NODE_FORANY: {
            tData collection = copiarData(eval(iterable));
            tData iterator = collection;
            int found = 0;

            while (iterator && found == 0) {
                tData elemento = get_dato(iterator);
                int booleano = cond ? get_bool_value(eval(cond)) : 1;

                if (booleano != 0) {
                    found = 1;
                    s->data = copiarData(elemento);
                    nuevo = eval(tblock);
                }
                iterator = get_next(iterator);
            }
            freeData(collection);
            break;
        }
        default:
            break;
    }
    return nuevo;
}

static tData eval_memory_ast(struct memory_ast *arbol) {
    if (!arbol) {
        tree_notify(ERR_SYS_NULL_POINTER, "Estructura memory_ast nula");
        return NULL;
    }

    tData nuevo = NULL;
    struct symbol* s = arbol->s;
    struct ast* a    = arbol->a;

    switch (get_nodetype((struct ast*)arbol)) {
        case NODE_ASIGNACIONMULTI: {
            tData coleccion_fuente = s->data;

            if (get_tipo(coleccion_fuente) != NODE_LIST && get_tipo(coleccion_fuente) != NODE_SET) {
                tree_notify(ERR_SEM_TYPE_MISMATCH, "Desestructuración múltiple requiere lista o conjunto");
                return NULL;
            }

            struct symlist* listaArgs = s->args;
            int tamArgs = compute_size(s->args);
            int tam = tamanio(coleccion_fuente);

            if (tamArgs != tam) {
                tree_notify(ERR_COL_INDEX_OUT_OF_BOUNDS, "Cantidad de variables no coincide con tamaño de colección");
                return NULL;
            }

            while (listaArgs) {
                // Se asigna la data del nodo interno al símbolo de la lista de argumentos
                if (listaArgs->s) {
                    listaArgs->s->data = copiarData(get_dato(coleccion_fuente));
                }
                listaArgs = listaArgs->next;
                coleccion_fuente = get_next(coleccion_fuente);
            }
            break;
        }

        case NODE_ASIGNACION: {
            nuevo = eval(a);
            s->data = copiarData(nuevo);
            break;
        }
        case NODE_VAR_REF: {
            nuevo = copiarData(s->data);
            if (nuevo == NULL) {
                tree_notify(ERR_GENERAL, "Se intentó acceder a una variable no inicializada");
            }
            break;
        }
        case NODE_FN_CALL:
            // A implementar a futuro con scopes
            break;

        default:
            break;
    }

    return nuevo;
}

static tData eval_list(struct ast *a) {
    if (!a) return NULL;

    tData nuevo = createData(NODE_LIST);
    if (!a->l) {
        return nuevo;
    }

    struct ast* nav = a->l;
    while (get_nodetype(nav) == NODE_LIST_OF_AST) {
        tData item = eval(nav->l);
        agregarData(&nuevo, item);
        nav = nav->r;
    }
    tData ultimo_item = eval(nav);
    agregarData(&nuevo, ultimo_item);
    return nuevo;
}

static tData eval_set(struct ast *a) {
    if (!a) return NULL;

    tData nuevo = createData(NODE_SET);
    if (!a->l) {
        return nuevo;
    }

    struct ast *nav = a->l;
    while (get_nodetype(nav) == NODE_LIST_OF_AST) {
        tData item = eval(nav->l);
        agregarData(&nuevo, item);
        nav = nav->r;
    }
    tData ultimo_item = eval(nav);
    agregarData(&nuevo, ultimo_item);
    return nuevo;
}

/*=======================================================================*/
/* DESPACHADOR PRINCIPAL EVAL                                            */
/*=======================================================================*/

tData eval(struct ast *a) {
    if (!a) {
        printf("Runtime Error: Puntero nulo en eval\n");
        return NULL;
    }

    int type = get_nodetype(a);

    if (type == NODE_INT || type == NODE_STR || type == NODE_DOUBLE || type == NODE_BOOL) {
        if (!a->d) {
            printf("Runtime Error: Literal sin dato asignado\n");
            exit(1);
        }
        return a->d;
    }

    // El switch despacha limpiamente a funciones semánticas dedicadas
    switch (type)
    {
        case '+': case '-': case '*': case '/':
            return eval_aritmetica(type, eval(a->l), eval(a->r));

        case NODE_MENOS_UNARIO: case NODE_MODULO:
            return eval_unario(type, eval(a->l));

        case NODE_MAYOR: case NODE_MENOR: case NODE_MAYOR_IGUAL: case NODE_MENOR_IGUAL:
            case NODE_IGUAL: case NODE_DISTINTO:
            return eval_relacional(type, eval(a->l), eval(a->r));

        case NODE_AND: case NODE_OR: case NODE_NOT:
            return eval_logica(type, a);

        case NODE_KICK: case NODE_ADD: case NODE_CONCAT: case NODE_TAKE: case NODE_GET:
            return eval_operacion_lista(type, a);

        case NODE_UNION: case NODE_INTER: case NODE_DIFF: case NODE_CONTAINS: case NODE_IN:
            return eval_operacion_conjunto(type, a);

        case NODE_LIST:
            return eval_list(a);

        case NODE_SET:
            return eval_set(a);

        case NODE_IF: case NODE_WHILE: case NODE_FORALL: case NODE_FORANY:
            return eval_flow((struct flow *)a);

        case NODE_ASIGNACION: case NODE_VAR_REF: case NODE_FN_CALL:
            return eval_memory_ast((struct memory_ast *)a);

        case NODE_BLOCK:
            eval(a->l);
            return eval(a->r);

        default:
            printf("Runtime Error: Tipo de nodo desconocido en eval: %d\n", type);
            break;
    }
    return NULL;
}
