#include "tree_data.h"

extern Arena evalArena;

/*==========================================================================*/
/*								CONSTRUCTORES								*/
/*==========================================================================*/
tData createData(int tipo)
{
	tData nvo= arenaAlloc(&evalArena, sizeof(TNodoData));

	if (!nvo) {
		tree_notify(ERR_SYS_NO_MEM_MEMORY, "Fallo crítico en malloc de createData");
		return NULL;
	}

	switch (tipo)
	{
	case NODE_STR:
		nvo->cad = (String_View){ .data = NULL, .count = 0 };
		break;

	case NODE_LIST:
	case NODE_SET:
		nvo->dato = NULL;
		nvo->sig = NULL;
		break;

	case NODE_INT:
		nvo->value = 0;
		break;

	case NODE_DOUBLE:
		nvo->real = 0.0;
		break;

	case NODE_BOOL:
		nvo->value = 0;
		break;

	default:
		printf("Error en createData tipo desconocido.\n");
		break;
	}
	nvo->tipoNodo = tipo;

	return nvo;
}

tData createStr(char *s)
{
	tData nvo = createData(NODE_STR);
	nvo->cad = sv(s);
	return nvo;
}
tData createInt(int value)
{
	tData nvo = createData(NODE_INT);
	nvo->value = value;
	return nvo;
}
tData createDouble(double real)
{
	tData nvo = createData(NODE_DOUBLE);
	nvo->real = real;
	return nvo;
}
tData createBool(char *s) {
	if (!s) {
		tree_notify(ERR_SYS_NULL_POINTER, "char* nulo en createBool");
		return NULL;
	}
	tData nvo = createData(NODE_BOOL);
	if (!nvo) return NULL;

	if (strcmp(s, "true") == 0) {
		nvo->value = 1;
		return nvo;
	}

	if (strcmp(s, "false") == 0) {
		nvo->value = 0;
		return nvo;
	}

	tree_notify(ERR_SEM_TYPE_MISMATCH, "Valor de cadena no válido para inicializar tipo Bool");
	return NULL;
}

tData createList() {
	return createData(NODE_LIST);
}

tData createSet() {
	return createData(NODE_SET);
}
/*==========================================================================*/
/*								GETTER Y NODE_SETTERS							*/
/*==========================================================================*/
int get_tipo(tData d) {
	return d ? d->tipoNodo : 0;
}

int get_value(tData d) {
	if (!d) return 0;
	return d->value;
}

double get_real(tData d) {
	if (!d) return 0.0;
	return d->real;
}

String_View get_cad(tData d) {
	if (!d) return (String_View){ .data = NULL, .count = 0 };
	return d->cad;
}

tData get_dato(tData d) {
	if (!d) return NULL;
	return d->dato;
}

tData get_next(tData d) {
	if (!d) return NULL;
	return d->sig;
}

void set_next(tData* origen, tData nuevo_sig) {
	if (origen && *origen) {
		(*origen)->sig = nuevo_sig;
	}
}
void set_dato(tData* origen, tData nuevo_dato) {
	if (origen && *origen) {
		(*origen)->dato = nuevo_dato;
	}
}

int get_bool_value(tData a) {
	if (!a) {
		tree_notify(ERR_SYS_NULL_POINTER, "en get_bool_value");
		return 0;
	}

	switch (get_tipo(a)) {
		case NODE_INT:
		case NODE_BOOL:
			return a->value;

		case NODE_DOUBLE:
			return (a->real == 0.0) ? 0 : 1;

		case NODE_STR:
			return 1;

		case NODE_LIST:
		case NODE_SET:
			return (a->dato == NULL) ? 0 : 1;

		default:
			tree_notify(ERR_RUN_GENERIC_EXECUTION, "tipoNodo desconocido en get_bool_value");
			return 0;
	}
}
/*==========================================================================*/
/*							OPERACIONES ARITMETICAS							*/
/*==========================================================================*/
tData sumaData(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en sumaData");
        return NULL;
    }
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->real + b->real);
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
        return createDouble(a->real + b->value);
    if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->value + b->real);

    return createInt(a->value + b->value);
}

tData restaData(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en restaData");
        return NULL;
    }
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->real - b->real);
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
        return createDouble(a->real - b->value);
    if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->value - b->real);

    return createInt(a->value - b->value);
}

tData prodData(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en prodData");
        return NULL;
    }
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->real * b->real);
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
        return createDouble(a->real * b->value);
    if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->value * b->real);

    return createInt(a->value * b->value);
}

tData cocData(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en cocData");
        return NULL;
    }

    if ((b->tipoNodo == NODE_INT && b->value == 0) || (b->tipoNodo == NODE_DOUBLE && b->real == 0.0)) {
        tree_notify(ERR_RUN_DIVISION_BY_ZERO, "en cocData");
        return NULL;
    }

    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->real / b->real);
    if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
        return createDouble(a->real / b->value);
    if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
        return createDouble(a->value / b->real);

    return createInt(a->value / b->value);
}

/*==========================================================================*/
/*							OPERACIONES RELACIONALES						*/
/*==========================================================================*/

tData compara_mayor(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_mayor");
        return NULL;
    }
    if (a->tipoNodo == NODE_STR && b->tipoNodo == NODE_STR) {
        return (sv_cmp(a->cad, b->cad) > 0) ? createBool("true") : createBool("false");
    }

    if ((a->tipoNodo == NODE_INT || a->tipoNodo == NODE_DOUBLE) &&
        (b->tipoNodo == NODE_INT || b->tipoNodo == NODE_DOUBLE)) {
        double val_a = (a->tipoNodo == NODE_INT) ? a->value : a->real;
        double val_b = (b->tipoNodo == NODE_INT) ? b->value : b->real;
        return (val_a > val_b) ? createBool("true") : createBool("false");
        }

    tree_notify(ERR_SEM_INVALID_TYPE, "Tipos incompatibles para comparación '>'");
    return createBool("false");
}

tData compara_menor(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_menor");
        return NULL;
    }
    if (a->tipoNodo == NODE_STR && b->tipoNodo == NODE_STR) {
        return (sv_cmp(a->cad, b->cad) < 0) ? createBool("true") : createBool("false");
    }

    double val_a = (a->tipoNodo == NODE_INT) ? a->value : a->real;
    double val_b = (b->tipoNodo == NODE_INT) ? b->value : b->real;

    return (val_a < val_b) ? createBool("true") : createBool("false");
}

tData compara_mayorigual(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_mayorigual");
        return NULL;
    }
    if (a->tipoNodo == NODE_STR && b->tipoNodo == NODE_STR) {
        return (sv_cmp(a->cad, b->cad) >= 0) ? createBool("true") : createBool("false");
    }

    double val_a = (a->tipoNodo == NODE_INT) ? a->value : a->real;
    double val_b = (b->tipoNodo == NODE_INT) ? b->value : b->real;

    return (val_a >= val_b) ? createBool("true") : createBool("false");
}

tData compara_menorigual(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_menorigual");
        return NULL;
    }
    if (a->tipoNodo == NODE_STR && b->tipoNodo == NODE_STR) {
        return (sv_cmp(a->cad, b->cad) <= 0) ? createBool("true") : createBool("false");
    }

    double val_a = (a->tipoNodo == NODE_INT) ? a->value : a->real;
    double val_b = (b->tipoNodo == NODE_INT) ? b->value : b->real;

    return (val_a <= val_b) ? createBool("true") : createBool("false");
}

tData compara_igual(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_igual");
        return NULL;
    }

    if ((a->tipoNodo == NODE_INT || a->tipoNodo == NODE_DOUBLE) &&
        (b->tipoNodo == NODE_INT || b->tipoNodo == NODE_DOUBLE)) {
        double val_a = (a->tipoNodo == NODE_INT) ? a->value : a->real;
        double val_b = (b->tipoNodo == NODE_INT) ? b->value : b->real;
        return (val_a == val_b) ? createBool("true") : createBool("false");
    }

    if (a->tipoNodo == NODE_BOOL && b->tipoNodo == NODE_BOOL) {
        return (get_bool_value(a) == get_bool_value(b)) ? createBool("true") : createBool("false");
    }

    return equalData(a, b) ? createBool("false") : createBool("true");
}

tData compara_distinto(tData a, tData b) {
    if (!a || !b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en compara_distinto");
        return NULL;
    }

    tData eq = compara_igual(a, b);
    tData res = get_bool_value(eq) ? createBool("false") : createBool("true");

    return res;
}
/*==========================================================================*/
/*				INFRAESTRUCTURA DE MEMORIA Y PROCESOS BÁSICOS     			*/
/*==========================================================================*/

void mostrarData(tData nodo) {
    if (!nodo) return;

    tData trav = NULL;

    switch (nodo->tipoNodo) {
        case NODE_INT:
            printf("%d", nodo->value);
            break;
        case NODE_DOUBLE:
            printf("%g", nodo->real);
            break;
        case NODE_BOOL:
            printf("%s", nodo->value ? "true" : "false");
            break;
        case NODE_STR:
            printf("%.*s", (int)nodo->cad.count, nodo->cad.data);
            break;
        case NODE_LIST:
            printf("[");
            trav = nodo;
            while (trav != NULL) {
                if (trav->dato) {
                    mostrarData(trav->dato);
                    if (trav->sig != NULL) printf(", ");
                }
                trav = trav->sig;
            }
            printf("]");
            break;
        case NODE_SET:
            printf("{");
            trav = nodo;
            while (trav != NULL) {
                if (trav->dato) {
                    mostrarData(trav->dato);
                    if (trav->sig != NULL) printf(", ");
                }
                trav = trav->sig;
            }
            printf("}");
            break;
        default: break;
    }
}

void agregarData(tData* cab, tData elem) {
    if (!cab || !(*cab) || (*cab)->tipoNodo == NODE_STR || !elem) return;

    if ((*cab)->dato == NULL) {
        (*cab)->dato = copiarData(elem);
        return;
    }

    tData aux;
    switch ((*cab)->tipoNodo) {
        case NODE_SET:
            aux = *cab;
            if (perteneceData(aux, elem) == 0) {
                while (aux->sig)
                    aux = aux->sig;
                aux->sig = createSet();
                aux = aux->sig;
                aux->dato = copiarData(elem);
            }
            break;
        case NODE_LIST:
            aux = *cab;
            while (aux->sig)
                aux = aux->sig;
            aux->sig = createList();
            aux = aux->sig;
            aux->dato = copiarData(elem);
            break;
        default: break;
    }
}

tData copiarData(tData copiado) {
    if (copiado == NULL) return NULL;

    tData nvo = NULL;

    switch (copiado->tipoNodo) {
        case NODE_INT:
            nvo = createInt(copiado->value);
            break;
        case NODE_DOUBLE:
            nvo = createDouble(copiado->real);
            break;
        case NODE_BOOL:
            nvo = createData(NODE_BOOL);
            if (nvo) nvo->value = copiado->value;
            break;
        case NODE_STR:
            nvo = createData(NODE_STR);
            nvo->cad = sv_copy(copiado->cad);
            break;
        case NODE_LIST:
            nvo = createList();
            if (nvo) {
                nvo->dato = copiarData(copiado->dato);
                nvo->sig = copiarData(copiado->sig);
            }
            break;
        case NODE_SET:
            nvo = createSet();
            if (nvo) {
                nvo->dato = copiarData(copiado->dato);
                nvo->sig = copiarData(copiado->sig);
            }
            break;
    }
    return nvo;
}

void freeData(tData descartado) {
    (void)descartado;
}

int equalData(tData A, tData B) {
    if (!A && !B) return 0;
    if (!A || !B || (A->tipoNodo != B->tipoNodo)) return 1;

    tData trav1 = NULL, trav2 = NULL;

    switch (A->tipoNodo) {
        case NODE_INT:
        case NODE_BOOL:
            return (A->value == B->value) ? 0 : 1;
        case NODE_DOUBLE:
            return (A->real == B->real) ? 0 : 1;
        case NODE_STR:
            return sv_cmp(A->cad, B->cad);
        case NODE_SET:
            trav1 = A;
            while (trav1) {
                if (trav1->dato && perteneceData(B, trav1->dato) == 0)
                    return 1;
                trav1 = trav1->sig;
            }
            trav1 = B;
            while (trav1) {
                if (trav1->dato && perteneceData(A, trav1->dato) == 0)
                    return 1;
                trav1 = trav1->sig;
            }
            return 0;
        case NODE_LIST:
            trav1 = A;
            trav2 = B;
            while (trav1 && trav2) {
                if (equalData(trav1->dato, trav2->dato) != 0)
                    return 1;
                trav1 = trav1->sig;
                trav2 = trav2->sig;
            }
            if (trav1 || trav2) return 1;
            return 0;
        default: break;
    }
    return 1;
}

int perteneceData(tData estructura, tData elem) {
    if (!estructura || !elem) return 0;

    tData trav = estructura;
    while (trav != NULL) {
        if (trav->dato && equalData(trav->dato, elem)==0) {
            return 1;
        }
        trav = trav->sig;
    }
    return 0;
}

int tamanioData(tData cab) {
    if (!cab || cab->dato == NULL) return 0;

    int cont = 0;
    tData trav = cab;
    while (trav != NULL) {
        if (trav->dato) cont++;
        trav = trav->sig;
    }
    return cont;
}
void eliminar_pos(tData* cab, int pos) {
    if (!cab || !(*cab) || (*cab)->dato == NULL) return;

    tData trav = *cab;

    if (pos == 0) {
        if (trav->sig == NULL) {
            freeData(trav->dato);
            trav->dato = NULL;
        } else {
            tData a_borrar = trav->sig;
            freeData(trav->dato);

            trav->dato = a_borrar->dato;
            trav->sig = a_borrar->sig;

            a_borrar->dato = NULL;
            a_borrar->sig = NULL;
        }
        return;
    }

    tData ant = NULL;
    int cont = 0;

    while (trav != NULL && cont < pos) {
        ant = trav;
        trav = trav->sig;
        cont++;
    }

    if (trav != NULL) {
        ant->sig = trav->sig;
        trav->sig = NULL;
        freeData(trav);
    }
}


/* ======================================================================= */
/* 5. INTERFACES DE LISTAS Y CONJUNTOS                                      */
/* ======================================================================= */
// Listas (Operaciones y accesos)
tData kickData(tData indice, tData lista) {
    if (!lista || !indice) {
        tree_notify(ERR_SYS_NULL_POINTER, "en kickData");
        return NULL;
    }
    if (lista->tipoNodo != NODE_LIST) {
        tree_notify(ERR_COL_EXPECTED_LIST, "en operación KICK");
        return NULL;
    }
    if (indice->tipoNodo != NODE_INT) {
        tree_notify(ERR_COL_EXPECTED_INT, "KICK requiere un índice entero");
        return NULL;
    }

    int pos = indice->value;
    tData resultado = lista;

    eliminar_pos(&resultado, pos);
    return resultado;
}

tData addData(tData elemento, tData lista) {
    if (!lista || !elemento) {
        tree_notify(ERR_SYS_NULL_POINTER, "en addData");
        return NULL;
    }
    if (lista->tipoNodo != NODE_LIST) {
        tree_notify(ERR_COL_EXPECTED_LIST, "en operación ADD");
        return NULL;
    }

    tData resultado = lista;
    agregarData(&resultado, elemento);
    return resultado;
}

tData takeData(tData indice, tData lista) {
    if (!lista || !indice) {
        tree_notify(ERR_SYS_NULL_POINTER, "en takeData");
        return NULL;
    }
    if (lista->tipoNodo != NODE_LIST) {
        tree_notify(ERR_COL_EXPECTED_LIST, "en operación TAKE");
        return NULL;
    }
    if (indice->tipoNodo != NODE_INT) {
        tree_notify(ERR_COL_EXPECTED_INT, "TAKE requiere un índice entero");
        return NULL;
    }

    int limite = indice->value;
    tData sublista = createList();

    tData trav = lista;
    int cont = 0;

    while (trav != NULL && cont <= limite) {
        if (trav->dato) {
            agregarData(&sublista, trav->dato);
        }
        trav = trav->sig;
        cont++;
    }
    return sublista;
}
tData concatData(tData lista_a, tData lista_b) {
    if (!lista_a || !lista_b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en concatData");
        return NULL;
    }
    if (lista_a->tipoNodo != NODE_LIST || lista_b->tipoNodo != NODE_LIST) {
        tree_notify(ERR_SEM_TYPE_MISMATCH, "CONCAT requiere que ambos operandos sean Listas");
        return NULL;
    }

    tData nueva_lista = copiarData(lista_a);
    tData trav = lista_b;

    while (trav != NULL) {
        if (trav->dato) {
            agregarData(&nueva_lista, trav->dato);
        }
        trav = trav->sig;
    }
    return nueva_lista;
}

// Conjuntos y Pertenencia General
tData getData(tData indice, tData coleccion) {
    if (!coleccion || !indice) {
        tree_notify(ERR_SYS_NULL_POINTER, "en getData");
        return NULL;
    }
    if (coleccion->tipoNodo != NODE_LIST && coleccion->tipoNodo != NODE_SET) {
        tree_notify(ERR_COL_EXPECTED_SET, "GET solo es válido para Listas o Conjuntos");
        return NULL;
    }
    if (indice->tipoNodo != NODE_INT) {
        tree_notify(ERR_COL_EXPECTED_INT, "El índice de acceso GET debe ser un entero");
        return NULL;
    }

    int pos_buscada = indice->value;
    tData trav = coleccion;
    int cont = 0;

    while (trav != NULL) {
        if (cont == pos_buscada) {
            return copiarData(trav->dato);
        }
        trav = trav->sig;
        cont++;
    }

    tree_notify(ERR_COL_INDEX_OUT_OF_BOUNDS, "en acceso GET");
    return NULL;
}

tData inData(tData elemento, tData contenedor) {
    if (!elemento || !contenedor) {
        tree_notify(ERR_SYS_NULL_POINTER, "en inData");
        return NULL;
    }

    if (contenedor->tipoNodo != NODE_SET && contenedor->tipoNodo != NODE_LIST) {
        tree_notify(ERR_COL_EXPECTED_LIST, "El operador IN requiere una lista o conjunto a la derecha");
        return NULL;
    }

    if (perteneceData(contenedor, elemento)) {
        return createBool("true");
    }
    return createBool("false");
}

tData unionData(tData conj_a, tData conj_b) {
    if (!conj_a || !conj_b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en unionData");
        return NULL;
    }

    if (conj_a->tipoNodo != NODE_SET || conj_b->tipoNodo != NODE_SET) {
        tree_notify(ERR_COL_EXPECTED_SET, "en operación Unión");
        return NULL;
    }

    tData C = createSet();
    tData aux = conj_a;
    while (aux) {
        if (aux->dato) {
            agregarData(&C, aux->dato);
        }
        aux = aux->sig;
    }

    aux = conj_b;
    while (aux) {
        if (aux->dato) {
            agregarData(&C, aux->dato);
        }
        aux = aux->sig;
    }
    return C;
}
tData diffData(tData conj_a, tData conj_b) {
    if (!conj_a || !conj_b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en diffData");
        return NULL;
    }

    if (conj_a->tipoNodo != NODE_SET || conj_b->tipoNodo != NODE_SET) {
        tree_notify(ERR_COL_EXPECTED_SET, "en operación Diferencia");
        return NULL;
    }

    tData C = createSet();
    tData aux = conj_a;

    while (aux) {
        if (aux->dato && !perteneceData(conj_b, aux->dato)) {
            agregarData(&C, aux->dato);
        }
        aux = aux->sig;
    }
    return C;
}
tData interData(tData conj_a, tData conj_b) {
    if (!conj_a || !conj_b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en interData");
        return NULL;
    }

    if (conj_a->tipoNodo != NODE_SET || conj_b->tipoNodo != NODE_SET) {
        tree_notify(ERR_COL_EXPECTED_SET, "en operación Intersección");
        return NULL;
    }

    tData C = createSet();
    tData aux = conj_a;

    while (aux) {
        if (aux->dato && perteneceData(conj_b, aux->dato)) {
            agregarData(&C, aux->dato);
        }
        aux = aux->sig;
    }
    return C;
}
tData containsData(tData conj_a, tData conj_b) {
    if (!conj_a || !conj_b) {
        tree_notify(ERR_SYS_NULL_POINTER, "en containsData");
        return NULL;
    }

    if (conj_a->tipoNodo != NODE_SET || conj_b->tipoNodo != NODE_SET) {
        tree_notify(ERR_COL_EXPECTED_SET, "en operación de Inclusión (contains)");
        return NULL;
    }

    tData aux = conj_b;
    while (aux) {
        if (aux->dato && !perteneceData(conj_a, aux->dato)) {
            return createBool("false");
        }
        aux = aux->sig;
    }
    return createBool("true");
}

tData negarData(tData a) {
    if (!a) {
        tree_notify(ERR_SYS_NULL_POINTER, "en negar_data");
        return NULL;
    }

    if (a->tipoNodo != NODE_INT && a->tipoNodo != NODE_DOUBLE) {
        tree_notify(ERR_SEM_NUMBERS_ONLY, "El operador de negación unaria (-) solo se aplica a números");
        return NULL;
    }

    if (a->tipoNodo == NODE_DOUBLE) {
        return createDouble(-(a->real));
    }
    return createInt(-(a->value));
}

tData absData(tData a) {
    if (!a) {
        tree_notify(ERR_SYS_NULL_POINTER, "en modulo_data");
        return NULL;
    }

    if (a->tipoNodo == NODE_INT) {
        int val = a->value;
        return createInt(val < 0 ? -val : val);
    }

    if (a->tipoNodo == NODE_DOUBLE) {
        double val = a->real;
        return createDouble(val < 0.0 ? -val : val);
    }

    if (a->tipoNodo == NODE_LIST || a->tipoNodo == NODE_SET) {
        return createInt(tamanioData(a));
    }

    tree_notify(ERR_SEM_TYPE_MISMATCH, "El operador de absoluto / longitud (|) no es válido para este tipo de dato");
    return NULL;
}
tData moduloData (tData a, tData b) {
    if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_INT) {
        if (b->value == 0) {
            tree_notify(ERR_RUN_DIVISION_BY_ZERO, "Módulo por cero");
            return NULL;
        }
        return createInt(a->value % b->value);
    }
    tree_notify(ERR_SEM_TYPE_MISMATCH, "El operador de módulo / longitud (|) no es válido para este tipo de dato");
    return NULL;
}