#ifndef TREE_DATA_H
#define TREE_DATA_H

#include "../common/string_view.h"
#include "tree_errors.h"
#include "tree_nodetypes.h"
#include "../common/arena.h"

/* Estructura Núcleo del Sistema de Tipos Unificado */
typedef struct nodo_data {
    int tipoNodo;
    union {
       String_View cad;
       int value;
       double real;
       struct {
          struct nodo_data * dato;
          struct nodo_data * sig;
       };
    };
} TNodoData;

typedef TNodoData * tData;

/* ======================================================================= */
/* 1. CONSTRUCTORES, GETTERS Y SETTERS                                     */
/* ======================================================================= */
tData createData(int tipo);
tData createInt(int val);
tData createDouble(double val);
tData createBool(char* val);
tData createStr(char* val);

/* Abstracciones para inicializar colecciones vacías */
tData createList();
tData createSet();

int get_tipo(tData d);
int get_value(tData d);
double get_real(tData d);
String_View get_cad(tData d);
int get_bool_value(tData d);

tData get_dato(tData d);
tData get_next(tData d);
void set_dato(tData* origen, tData nuevo_dato);
void set_next(tData* origen, tData nuevo_sig);

/* ======================================================================= */
/* 2. OPERACIONES ARITMÉTICAS Y UNARIAS                                    */
/* ======================================================================= */
tData sumaData(tData a, tData b);
tData restaData(tData a, tData b);
tData prodData(tData a, tData b);
tData cocData(tData a, tData b);
tData negar_data(tData a);
tData moduloData(tData a);

/* ======================================================================= */
/* 3. OPERACIONES RELACIONALES                                             */
/* ======================================================================= */
tData compara_mayor(tData a, tData b);
tData compara_menor(tData a, tData b);
tData compara_mayorigual(tData a, tData b);
tData compara_menorigual(tData a, tData b);
tData compara_igual(tData a, tData b);
tData compara_distinto(tData a, tData b);

/* ======================================================================= */
/* 4. INFRAESTRUCTURA DE MEMORIA Y PROCESOS BÁSICOS                        */
/* ======================================================================= */
int equalData(tData a, tData b);
void mostrarData(tData d);
void freeData(tData d);
tData copiarData(tData d);
void agregarData(tData* cab, tData elem);
int perteneceData(tData estructura, tData elem);
int tamanioData(tData cab);

/* ======================================================================= */
/* 5. INTERFACES DE LISTAS Y CONJUNTOS                                      */
/* ======================================================================= */

// Listas (Operaciones y accesos)
tData kickData(tData indice, tData lista);
tData addData(tData elemento, tData lista);
tData takeData(tData indice, tData lista);
tData concatData(tData lista_a, tData lista_b);

// Conjuntos y Pertenencia General
tData getData(tData indice, tData conjunto);
tData inData(tData elemento, tData contenedor);
tData unionData(tData conj_a, tData conj_b);
tData diffData(tData conj_a, tData conj_b);
tData interData(tData conj_a, tData conj_b);
tData containsData(tData conj_a, tData conj_b);

// Extras
tData negarData (tData a);
tData moduloData (tData a);

#endif