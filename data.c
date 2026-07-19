#include "data.h"
#include "include/tree_nodetypes.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
/*==========================================================================*/
/*								CONSTRUCTORES								*/
/*==========================================================================*/
tData createData(int tipo)
{
	tData nvo; 
	nvo = (tData)malloc(sizeof(TNodoData));

	switch (tipo)
	{
	case NODE_STR:
		nvo->cad = create();
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
	nvo->cad = load2(s);
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
tData createBool(char *s)
{
	if (!s)
	{
		printf("char* null en create bool ");
		return NULL;
	}
	int i, b;
	tData nvo = createData(NODE_BOOL);

	char *_true = "true";
	i = 0;
	b = 1;
	while (i < 4 && b)
	{
		if (s[i] != _true[i])
			b = 0;
		i++;
	}
	if (b)
	{
		nvo->value = 1;
		return nvo;
	}

	char *_false = "false";
	i = 0;
	b = 1;
	while (i < 4 && b)
	{
		if (s[i] != _false[i])
			b = 0;
		i++;
	}
	if (b)
	{
		nvo->value = 0;
		return nvo;
	}
	printf("error %s no es una valor valido para createBool", s);
	return NULL;
}

/*==========================================================================*/
/*								GETTER Y NODE_SETTERS							*/
/*==========================================================================*/
tData get_dato(tData a)
{
	if (get_tipo(a) == NODE_LIST || get_tipo(a) == NODE_SET)
	{
		return a->dato;
	}
	else
	{
		printf("error %d no tiene campo dato", get_tipo(a));
		return NULL;
	}
}
tData get_next(tData a)
{
	if (get_tipo(a) == NODE_LIST || get_tipo(a) == NODE_SET)
	{
		return a->sig;
	}
	else
	{
		printf("error %d no tiene campo sig", get_tipo(a));
		return NULL;
	}
}
void set_next(tData *node, tData next)
{
	if(!(*node))
	{
		printf("error en set_next puntero null");
		return;
	}
	if(get_tipo(*node) != NODE_SET && get_tipo(*node) != NODE_LIST)
	{
		printf("set_next solo funciona para nodotipo NODE_LIST NODE_SET");
		return;
	}
	(*node)->sig = next;
}
void set_dato(tData *node, tData dato)
{
	if(!(*node))
	{
		printf("error en set_dato puntero null");
		return;
	}
	if(get_tipo(*node) != NODE_SET && get_tipo(*node) != NODE_LIST)
	{
		printf("set_dato solo funciona para nodotipo NODE_LIST NODE_SET");
		return;
	}
	(*node)->dato = dato;
}

int get_tipo(tData a)
{
	if(!a)
	{
		printf("get_tipo recibe puntero null");
		return 0;
	}
	return a->tipoNodo;
}
int get_bool_value(tData a){
	if(!a)
	{
		//printf("\nEs aqui\n");
		printf("puntero null en get_bool_value\n");
		return 0;
	}
	int bool_value;
	switch (get_tipo(a))
	{
	case NODE_INT: case NODE_BOOL:
		bool_value = get_value(a);
		break;
	case NODE_DOUBLE:
		if(get_real(a) == 0.0)
		{
			bool_value = 0;
		}
		else 
		{
			bool_value = 1;
		}
		break;
	case NODE_STR:
		bool_value = 1;
		break;
	case NODE_LIST: case NODE_SET:
		if(get_dato(a) == NULL)
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;
	default:
		printf("tipoNodo desconocido get_bool_value\n");
		bool_value = 0;
		break;
	}
	return bool_value;
}

int get_value(tData a)
{
	if (!a)
	{
		printf("\nQuiere obtener un valor de un tdata vacio.\n");
		exit(1);
	}
	if(get_tipo(a) != NODE_INT && get_tipo(a) != NODE_BOOL)
	{
		printf("error en get_value solo para nodotipo NODE_INT o NODE_BOOL\n");
		return 0;
	}
	return a->value;
}
double get_real(tData a)
{
	if (get_tipo(a) != NODE_DOUBLE)
	{
		printf("error %d no tiene campo real", get_tipo(a));
		return 0;
	}
	else
	{
		return a->real;
	}
}
str get_cad(tData a)
{
	if (get_tipo(a) != NODE_STR)
	{
		printf("error %d no tiene campo cad", get_tipo(a));
		return 0;
	}
	else
	{
		return a->cad;
	}
}

/*==========================================================================*/
/*							OPERACIONES ARITMETICAS							*/
/*==========================================================================*/
tData sumaData(tData a, tData b)
{
	if (!a || !b)
	{
		printf("punteros nulos en sumaData");
		return NULL;
	}
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real + b->real);
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
		return createDouble(a->real + b->value);
	if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real + b->value);
	return createInt(a->value + b->value);
}
tData restaData(tData a, tData b)
{
	if (!a || !b)
	{
		printf("punteros nulos en restaData");
		return NULL;
	}
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real - b->real);
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
		return createDouble(a->real - b->value);
	if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real - b->value);
	return createInt(a->value - b->value);
}
tData prodData(tData a, tData b)
{
	if (!a || !b)
	{
		printf("punteros nulos en prodData");
		return NULL;
	}
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real * b->real);
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
		return createDouble(a->real * b->value);
	if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real * b->value);
	return createInt(a->value * b->value);
}
tData cocData(tData a, tData b)
{
	if (!a || !b)
	{
		printf("punteros nulos en divData");
		return NULL;
	}
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real / b->real);
	if (a->tipoNodo == NODE_DOUBLE && b->tipoNodo == NODE_INT)
		return createDouble(a->real / b->value);
	if (a->tipoNodo == NODE_INT && b->tipoNodo == NODE_DOUBLE)
		return createDouble(a->real / b->value);
	return createInt(a->value / b->value);
}

/*==========================================================================*/
/*							OPERACIONES RELACIONALES							*/
/*==========================================================================*/
tData compara_mayor(tData A, tData B){
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_mayor\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: nuevo = (get_value(A) > get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) > get_real(B)) ?  createBool("true"): createBool("false"); break;
	
	case NODE_SET: case NODE_LIST: case NODE_BOOL:
	{
		printf("Error: No se pueden comparar usando mayor las estos tipos de datos \n");
		break;
	}
	
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}
tData compara_menor(tData A, tData B)
{	
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_menor\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: nuevo = (get_value(A) < get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) < get_real(B)) ?  createBool("true"): createBool("false"); break;
	
	case NODE_SET: case NODE_LIST: case NODE_BOOL:
	{
		printf("Error: No se pueden comparar usando menor las estos tipos de datos \n");
		break;
	}
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}
tData compara_mayorigual(tData A, tData B)
{
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_mayorigual\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: nuevo = (get_value(A) >= get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) >= get_real(B)) ?  createBool("true"): createBool("false"); break;
	
	case NODE_SET: case NODE_LIST: case NODE_BOOL:
	{
		printf("Error: No se pueden comparar usando mayor-igual las estos tipos de datos \n");
		break;
	}
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}
tData compara_menorigual(tData A, tData B){
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_menorigual\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: nuevo = (get_value(A) <= get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) <= get_real(B)) ?  createBool("true"): createBool("false"); break;
	
	case NODE_SET: case NODE_LIST: case NODE_BOOL:
	{
		printf("Error: No se pueden comparar usando menor-igual las estos tipos de datos \n");
		break;
	}
	
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}
tData compara_igual(tData A, tData B)
{
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_igual\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: case NODE_BOOL: nuevo = (get_value(A) == get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) == get_real(B)) ?  createBool("true"): createBool("false"); break;
	case NODE_SET: case NODE_LIST: 
	{
		nuevo = (Igualdad(A,B) == 0) ? createBool("true") : createBool("false");
		break;
	}
	case NODE_STR:
		{
			nuevo = (!compStr(get_cad(A), get_cad(B))) ? createBool("true") : createBool("false");
		}
		break;
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}
tData compara_distinto(tData A, tData B)
{
	tData nuevo;
	if(!A || !B)
	{
		printf("puntero null en compara_distinto\n");
		return NULL;
	}
	if(get_tipo(A) != get_tipo(B))
	{
		printf("las comparaciones deben tener operandos de mismo tipo\n");
		return NULL;
	}
	
	switch (get_tipo(A))
	{
	case NODE_INT: case NODE_BOOL: nuevo = (get_value(A) != get_value(B)) ? createBool("true") : createBool("false"); break;
	case NODE_DOUBLE: nuevo = (get_real(A) != get_real(B)) ?  createBool("true"): createBool("false"); break;
	case NODE_SET: case NODE_LIST: 
	{
		nuevo = (Igualdad(A,B) == 1) ? createBool("true") : createBool("false");
		break;
	}
	case NODE_STR:
		{
			nuevo = (compStr(get_cad(A), get_cad(B))) ? createBool("true") : createBool("false");
		}
		break;
	default: printf("Error: hola diablo\n"); break;
	}
	return nuevo;
}

/*==========================================================================*/
/*							MODULOS BASICOS TDATA							*/
/*==========================================================================*/
void mostrarData(tData nodo)
{
	if (nodo == NULL)
	{
		// printf("_mostrando vacio_\n");
		return;
	}


	switch (nodo->tipoNodo)
	{
	case NODE_STR:
		print(nodo->cad);
		break;
	case NODE_INT:
		printf("%d", nodo->value);
		break;
	case NODE_DOUBLE:
		printf("%f", nodo->real);
		break;
	case NODE_BOOL:
		(nodo->value == 0) ? printf("false") : printf("true");
		break;
	case NODE_LIST:
		printf("[");

		tData aux;
		aux = nodo;
		while (aux != NULL)
		{
			mostrarData(aux->dato);
			if (aux->sig != NULL)
				printf(",");
			aux = aux->sig;
		}
		printf("]");

		break;
	case NODE_SET:
		printf("{");

		tData aux2;
		aux2 = nodo;
		while (aux2 != NULL)
		{
			mostrarData(aux2->dato);
			if (aux2->sig != NULL)
				printf(",");
			aux2 = aux2->sig;
		}
		printf("}");
		break;
	}
}
void agregarData(tData *cabe, tData elem)
{
	tData cab = *cabe;
	if (!cab)
	{
		printf("No se puede agregar un elemento a un espacio de memoria no asignado");
		return;
	}
	if (get_tipo(cab) != NODE_SET && get_tipo(cab) != NODE_LIST)
	{
		printf("No se puede agregar un elemento a un objeto que no es list o conjunto");
		return;
	}
	if (cab->dato == NULL)
	{
		cab->dato = elem; // copiarlo
	}
	else
	{
		switch (cab->tipoNodo)
		{
		case NODE_STR:
		case NODE_INT:
		case NODE_DOUBLE:
		case NODE_BOOL:
			break;
		case NODE_SET:
		case NODE_LIST:
		{
			tData nvo;

			if (cab->tipoNodo == NODE_SET)
			{
				if (pertenece(cab, elem) == 0)
					return;
			}
			// printf("Estoy en agregarData\n");

			while (cab->sig != NULL)
				cab = cab->sig;
			nvo = createData(cab->tipoNodo);
			nvo->dato = copiarData(elem);
			cab->sig = nvo;
			break;
		}
		default:
			printf("caso no def en agregarData");
			break;
		}
	}
	// printf("\n\ndespues de agregar elem: ");
	// mostrarData(elem);
	// printf("\nla lista cab quedo: ");
	// mostrarData(*cabe);
	// printf("\n");
}
tData copiarData(tData copiado)
{
	tData nvo = NULL;
	if (copiado == NULL)
		return NULL;
	switch (copiado->tipoNodo)
	{
	case NODE_INT:
		nvo = createInt(copiado->value);
		break;
	case NODE_DOUBLE:
		nvo = createDouble(copiado->real);
		break;
	case NODE_BOOL:
		if (get_value(copiado))
		{
			nvo = createBool("true");
		}
		else
		{
			nvo = createBool("false");
		}
		break;
	case NODE_STR:
		nvo = createData(NODE_STR);
		nvo->cad = copyStr(copiado->cad);
		break;
	case NODE_LIST:
		nvo = createData(NODE_LIST);
		nvo->dato = copiarData(copiado->dato);
		nvo->sig = copiarData(copiado->sig);
		break;
	case NODE_SET:
		nvo = createData(NODE_SET);
		nvo->dato = copiarData(copiado->dato);
		nvo->sig = copiarData(copiado->sig);
		break;
	}

	return nvo;
}
void freeData(tData descartado)
{

	if (descartado == NULL)
		return;

	switch (descartado->tipoNodo)
	{
	case NODE_STR:
		freeString(descartado->cad);
		break;
	case NODE_LIST:
	case NODE_SET:
		freeData(descartado->dato);
		freeData(descartado->sig);
		break;
	default: break;
	}

	free(descartado);
}
int Igualdad(tData A, tData B)
{

	if (A == NULL || B == NULL)
	{
		printf("Igualdad recibe punteros nulo\n");
		return 0;
	}
	if ((A->tipoNodo != B->tipoNodo))
	{
		return 1;
	}

	switch (A->tipoNodo)
	{
	case NODE_STR:
		return compStr(A->cad, B->cad);
		break;
	case NODE_BOOL:
	case NODE_INT:
		return !(get_value(A) == get_value(B));
	case NODE_DOUBLE:
		return !(get_real(A) == get_real(B));
	case NODE_SET:
	{
		tData auxA = A;
		while (auxA != NULL)
		{
			if (pertenece(B, auxA->dato) != 0) // 0 pertenece. 1 no pertenece
				return 1; //distintos
			auxA = auxA->sig;
		}
		tData auxB = B;
		while (auxB != NULL)
		{
			if (pertenece(A, auxB->dato) != 0)
				return 1;
			auxB = auxB->sig;
		}
		return 0; // iguales. todo A e B, todo B e A
	}

	case NODE_LIST:
	{
		tData auxA = A;
		tData auxB = B;

		while (auxA != NULL && auxB != NULL)
		{
			if (Igualdad(auxA->dato, auxB->dato) != 0)
				return 1; //1 no son iguales, 0 iguales
			auxA = auxA->sig;
			auxB = auxB->sig;
		}
		if (auxA != NULL || auxB != NULL)
			return 1; // si uno es mas largo que el otro

		return 0; // iguales
	}
	default:
		printf("tipoNodo desconocido\n");
		return 1;
	}
}

/*==========================================================================*/
/*							OPERACIONES CONJUNTOS							*/
/*==========================================================================*/

tData Union(tData A, tData B)
{
	if (A == NULL )
	{
		return copiarData(B);
	}
	if ( get_dato(A) == NULL )
	{
		return copiarData(B); // B es el conj => AuB = A
	}
	if (B == NULL )
	{
		return copiarData(A);
	}
	if ( get_dato(B) == NULL )
	{
		return copiarData(A); // B es el conj => AuB = A
	}

	if (A->tipoNodo != NODE_SET || B->tipoNodo != NODE_SET)
		return NULL;

	tData C_Cab = NULL, C_act = NULL;
	tData aux;

	while (A != NULL)
	{

		aux = createData(NODE_SET);
		aux->dato = copiarData(A->dato);

		if (C_Cab == NULL)
			C_Cab = aux;
		else
			C_act->sig = aux;

		C_act = aux;

		A = A->sig;
	}
	
	while (B != NULL)
	{
		if ( pertenece(C_Cab, B->dato) )  // 1 si no pertence => agrega
		{
			
			aux = createData(NODE_SET);
			aux->dato = copiarData(B->dato);

			if (C_Cab == NULL)
				C_Cab = aux;
			else
				C_act->sig = aux;

			C_act = aux;
		}
		B = B->sig;
	}

	return C_Cab;
}
tData Interseccion(tData A, tData B)
{
	if (A == NULL || B == NULL)
		return NULL;

	if (A->tipoNodo != NODE_SET || B->tipoNodo != NODE_SET)
		return NULL;
	tData C_Cab = NULL, C_act = NULL;
	tData aux;
	while (A != NULL)
	{

		if (!pertenece(B, A->dato))
		{

			aux = createData(NODE_SET);
			aux->dato = copiarData(A->dato);

			if (C_Cab == NULL)
				C_Cab = aux;
			else
				C_act->sig = aux;

			C_act = aux;
		}
		A = A->sig;
	}
	return C_Cab;
}
tData Diferencia(tData A, tData B)
{
	if (A == NULL)
		return NULL;
	if (B == NULL)
		return copiarData(A);

	if (A->tipoNodo != NODE_SET || B->tipoNodo != NODE_SET)
		return NULL;

	tData C_Cab = NULL, C_act = NULL;
	tData aux;
	while (A != NULL)
	{

		if (pertenece(B, A->dato))
		{

			aux = createData(NODE_SET);
			aux->dato = copiarData(A->dato);

			if (C_Cab == NULL)
				C_Cab = aux;
			else
				C_act->sig = aux;

			C_act = aux;
		}
		A = A->sig;
	}
	return C_Cab;
}
tData DifSimetrica(tData A, tData B)
{
	if (A->tipoNodo != NODE_SET || B->tipoNodo != NODE_SET)
		return NULL;

	tData D = Diferencia(A, B);
	tData E = Diferencia(B, A);

	tData F = NULL;
	F = Union(D, E);

	return F;
}
int pertenece(tData A, tData elem)
{
	if ( A == NULL  )
	{
		printf("pertenece recibe conj: puntero nulo\n");
		return 1;
	}
	if ( elem == NULL)
	{
		//printf("pertenece recibe punteros nulos\n");
		return 1;
	}
	if (A->tipoNodo != NODE_LIST && A->tipoNodo != NODE_SET){
		printf("pertenece no recibe un conjunto o lista\n");
		return 1;
	}

	tData aux = A;

	while (aux != NULL)
	{
		//if (! aux->dato){printf("conj vacio\n");}
		if (Igualdad(aux->dato, elem) == 0) // si son iguañes
			return 0; // si pertenece
		aux = aux->sig;
	}

	return 1; // no pertenece
}
int pertenece_completing(tData elem, tData A)
{
	int band = 0; // elem no pertenece
	while (A != NULL && !band)
	{
		if ( Igualdad(elem, get_dato(A)) == 0 ) // pertenece(a,b)
			band = 1;  // elem si pertenece //no b=0
		A = get_next(A);
	}
	return band;
}
int contenido(tData A, tData B)
{

	tData auxA = A;
	while (auxA != NULL) // A c B <=> VaeA | aeB
	{
		if (pertenece(B, auxA->dato) != 0) // 0 pertence. 1 no pertenece
			return 0; 
		auxA = auxA->sig;
	}
	return 1;
}

/*==========================================================================*/
/*							OPERACIONES NODE_LISTA								*/
/*==========================================================================*/
tData concat_list(tData a, tData b)
{
	tData nuevo = createData(NODE_LIST);
	if (!a || !b)
	{
		printf("error concatenar_listas  de listas nulas");
		return NULL;
	}

	while (a != NULL)
	{
		agregarData(&nuevo, get_dato(a));
		a = get_next(a);
	}
	
	while (b != NULL)
	{
		agregarData(&nuevo, get_dato(b));
		b = get_next(b);
	}
	
	return nuevo;
}
void eliminar_pos(tData *l, int pos)
{
	int i;
	tData ant, nav;
	nav = *l;
	ant = NULL;

	if (!nav)
	{
		printf("Error eliminar_pos nav null");
		return ;
	}
	if ( get_tipo(nav) != NODE_LIST )
	{
		printf("Error eliminar_pos es una operacion de listas");
		return;
	}

	for (i = 1; i < pos && nav; i++)
	{
		ant = nav;
		nav = get_next(nav);
	}
	if (!nav)
		return; // posicion fuera de la lista
	if (!ant)
	{ // eliminar cabeza

		*l = get_next(nav);
		if (*l == NULL)
		{ // corresponde lista vacia
			*l = createData(NODE_LIST);
		}
		//freeData(get_dato(nav));
		//freeData(nav);
	}
	else
	{ // eliminacion en lista
		set_next(&ant, get_next(nav));
		//freeData(nav);
	}
}

/*==========================================================================*/
/*					   OPERACIONES NODE_LISTA Y CONJUNTOS						*/
/*==========================================================================*/
tData elemento_pos(tData data, int pos)
{
	if (!data)
	{
		printf("puntero nulo en elemento_pos\n");
		return NULL;
	}
	if (get_tipo(data) != NODE_SET && get_tipo(data) != NODE_LIST)
	{
		printf("Error elemento_pos es una operacion de listas o conjuntos\n");
		return NULL;
	}
	int i = 1;
	while (i < pos && data != NULL)
	{
		i++;
		data = get_next(data);
	}
	if (i == pos && data != NULL)
		return get_dato(data);
	printf("Posicion más grande que lista.\n");
	return createStr("-1"); // createInt(-1);
}
int tamanio(tData A)
{
	if ( !A ) 
	{
		printf("Error interno tamanio puntero null\n");
		return 0;
	}
	
	if ( A->tipoNodo != NODE_SET && A->tipoNodo != NODE_LIST )
		return 1;
	

	int c = 0;
	if ( get_next(A) == NULL )
	{
		if ( get_dato(A) == NULL )
		{
			return 0; // lista vacia
	
		}
	}
	while (A != NULL)
	{
		c++;
		A = A->sig;
	}

	return c;
}
tData negar_data(tData data)
{
	tData nuevo = NULL;
	if(!data)
	{
		return NULL;
	}
	switch (get_tipo(data))
    {
        case NODE_STR:
		{
			nuevo = copiarData(data);
			break;
		}
		case NODE_INT: 
        {
            nuevo = createInt(-get_value(data));
            break;
        } 
        case NODE_DOUBLE:
        {
            nuevo = createDouble(-get_real(data));
            break;
        }
        case NODE_BOOL:
        {
            if(get_value(data))
            {
                nuevo = createBool("false");
            }
            else
            {
                nuevo = createBool("true");
            }
            break;
        }
        case NODE_LIST: 
        {
            nuevo = createData(NODE_LIST);
			nuevo->dato = negar_data(data->dato);
			nuevo->sig = negar_data(data->sig);
            break;
        }         
		case NODE_SET:
		{
			nuevo = createData(NODE_SET);
			nuevo->dato = negar_data(data->dato);
			nuevo->sig = negar_data(data->sig);
            break;
		}
        default:
		{
			printf("nodotipo desconocido en negar_data\n");
            break;
		}
    }
	return nuevo;
}
tData modulo_data(tData data)
{
	tData modulo_data;
	if(!data)
	{
		printf("puntero null en modulo_data\n");
		return 0;
	}
	if(get_tipo(data) == NODE_BOOL)
	{
		printf("Error en modulo_data data es de tipo bool\n");
		return NULL;
	}
	switch (get_tipo(data))
	{ 
	case NODE_INT:
	{ 
		modulo_data = createInt(abs(get_value(data)));
		break;
	}
	case NODE_DOUBLE:
	{
		modulo_data = createDouble(fabs(get_real(data)));
		break;
	}
	case NODE_STR:
	{
		modulo_data = createInt(1);
		break;
	}
	case NODE_LIST: case NODE_SET:
	{
		modulo_data = createInt(tamanio(data));
		break;
	}
	default:
	{
		printf("tiponodo desconocido en modulo_data\n");
		break;
	}
	}

	
	return modulo_data;
}
