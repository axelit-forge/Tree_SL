#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "data.h"
#include "include/tree_nodetypes.h"

// 1. Inyectamos la lógica de errores una sola vez
#define TREE_ERRORS_IMPLEMENTATION
#include "include/tree_errors.h"

// 2. Inyectamos el AST
#define TREE_AST_IMPLEMENTATION
#include "include/tree_ast.h"

// 3. Inyectamos la Tabla de Símbolos
#define TREE_SYMTAB_IMPLEMENTATION
#include "tree_symtab.h"

// 4. Inyectamos el Eval
#define TREE_EVAL_IMPLEMENTATION
#include "include/tree_eval.h"
