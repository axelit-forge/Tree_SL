#include "include/tree_eval.h"
#include "include/tree_ast.h"
#include "include/tree_symtab.h"
#include "include/tree_errors.h"
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
    // TODO: Migrar MENOS_UNARIO y MODULO
    return NULL;
}

static tData eval_logica(int op, struct ast *a) {
    // TODO: Migrar AND, OR, NOT e implementar cortocircuitos si querés
    return NULL;
}

static tData eval_operacion_lista(int op, struct ast *a) {
    // TODO: Migrar KICK, ADD, CONCAT, TAKE, GET y sus validaciones de lista
    return NULL;
}

static tData eval_operacion_conjunto(int op, struct ast *a) {
    // TODO: Migrar UNION, INTER, DIFF, CONTAINS, IN y validaciones de conjunto
    return NULL;
}

tData eval_flow(struct flow *a) {
    // TODO: Migrar las estructuras IF, WHILE, FORALL, FORANY
    return NULL;
}

tData eval_memory_ast(struct memory_ast *arbol) {
    // TODO: Migrar ASIGNACION, VAR_REF, FN_CALL, ASIGNACIONMULTI
    return NULL;
}

tData eval_list(struct ast *a) {
    // TODO: Migrar la evaluación de literales de lista [exps]
    return NULL;
}

tData eval_set(struct ast *a) {
    // TODO: Migrar la evaluación de literales de conjunto {exps}
    return NULL;
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
