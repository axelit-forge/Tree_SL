#include "tree_errors.h"
#include <stdio.h>
#include <stdlib.h>

void tree_notify(TreeErrorCode code, const char* extra_info) {
    const char* error_type = "ERROR";
    int is_fatal = 0;

    if (code >= ERR_SYS_START && code < ERR_SEM_START) { error_type = "SISTEMA/CRÍTICO"; is_fatal = 1; }
    else if (code >= ERR_SEM_START && code < ERR_COL_START) { error_type = "SEMÁNTICO"; }
    else if (code >= ERR_COL_START && code < ERR_RUN_START) { error_type = "COLECCIÓN"; }
    else if (code >= ERR_RUN_START) { error_type = "EJECUCIÓN"; is_fatal = 1; }

    fprintf(stderr, "\n[%s - Código %d]: ", error_type, code);

    switch(code) {
        case ERR_SYS_NULL_NODETYPE:   fprintf(stderr, "Puntero nulo en get_nodetype().\n"); break;
        case ERR_SYS_NO_MEM_AST:      fprintf(stderr, "Falta de memoria al construir un nodo AST (newast).\n"); break;
        case ERR_SYS_NO_MEM_FLOW:     fprintf(stderr, "Falta de memoria al construir un flujo de control (newflow).\n"); break;
        case ERR_SYS_NO_MEM_MEMORY:   fprintf(stderr, "Falta de memoria en estructura de asignación (newmemory_ast).\n"); break;
        case ERR_SYS_NULL_POINTER:    fprintf(stderr, "Se detectó un puntero nulo inesperado.\n"); break;
        case ERR_SYS_OVERFLOW_SYMTAB: fprintf(stderr, "Desbordamiento crítico de la tabla de símbolos.\n"); break;
        case ERR_SEM_UNINITIALIZED_VAR: fprintf(stderr, "La variable solicitada no ha sido inicializada.\n"); break;
        case ERR_SEM_NUMBERS_ONLY:       fprintf(stderr, "Operación matemática inválida. Solo se admiten tipos INT o DOUBLE.\n"); break;
        case ERR_SEM_TYPE_MISMATCH:      fprintf(stderr, "Conflicto de tipos. Los operandos deben ser del mismo tipo.\n"); break;
        case ERR_COL_INDEX_OUT_OF_BOUNDS: fprintf(stderr, "Índice fuera de rango. La posición excede el tamaño actual de la estructura.\n"); break;
        case ERR_COL_EXPECTED_INT:        fprintf(stderr, "Tipo incorrecto para índice. Se esperaba un entero (INT).\n"); break;
        case ERR_COL_EXPECTED_LIST:       fprintf(stderr, "Operación exclusiva de Listas. Se esperaba una estructura tipo [].\n"); break;
        case ERR_COL_EXPECTED_SET:        fprintf(stderr, "Operación exclusiva de Conjuntos. Se esperaba una estructura tipo {}.\n"); break;
        case ERR_RUN_DIVISION_BY_ZERO:    fprintf(stderr, "Error matemático: División por cero.\n"); break;
        case ERR_SEM_INVALID_TYPE:      fprintf(stderr, "Error semantico:.\n"); break;
        default: fprintf(stderr, "Error general de ejecución.\n"); break;
    }

    if (extra_info) fprintf(stderr, "   Contexto: %s\n", extra_info);
    if (is_fatal) { fprintf(stderr, "[FATAL]: Abortando programa.\n"); exit(1); }
}

int yyerror(char* s) {
    fprintf(stderr, "\n[ERROR SINTÁCTICO]: %s\n", s);
    return 0;
}