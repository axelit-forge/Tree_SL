#ifndef TREE_NODETYPES_H
#define TREE_NODETYPES_H

typedef enum {
    NODE_AST_START = 256,

    // --- Literales y Tipos de Datos ---
    NODE_STR,
    NODE_LIST,
    NODE_SET,
    NODE_INT,
    NODE_DOUBLE,
    NODE_BOOL,

    // --- Expresiones Aritméticas Complejas ---
    NODE_MENOS_UNARIO,
    NODE_MODULO,

    // --- Operaciones de Listas ---
    NODE_ADD,
    NODE_KICK,
    NODE_TAKE,
    NODE_CONCAT,

    // --- Operaciones de Conjuntos ---
    NODE_UNION,
    NODE_INTER,
    NODE_DIFF,
    NODE_CONTAINS,
    NODE_IN,
    NODE_GET,

    // --- Estructuras Internas ---
    NODE_LIST_OF_AST,
    NODE_BLOCK,

    // --- Flujo de Control ---
    NODE_IF,
    NODE_WHILE,
    NODE_FORALL,
    NODE_FORANY,
    NODE_PRINT,

    // --- Memoria y Funciones ---
    NODE_VAR_REF,
    NODE_ASIGNACION,
    NODE_FN_DEFINITION,
    NODE_FN_CALL,
    NODE_ASIGNACIONMULTI,

    // --- Expresiones Relacionales ---
    NODE_MAYOR,
    NODE_MENOR,
    NODE_MAYOR_IGUAL,
    NODE_MENOR_IGUAL,
    NODE_IGUAL,
    NODE_DISTINTO,

    // --- Expresiones Lógicas ---
    NODE_AND,
    NODE_OR,
    NODE_NOT

} TreeNodeType;

#endif