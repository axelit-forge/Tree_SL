#include <stdio.h>
#include "arena.h"
#include "tree_symtab.h"

Arena astArena;
Arena evalArena;
int yyparse(void);

int main(void) {
    astArena  = arenaCreate(1024 * 1024); //1 mb para cada arena
    evalArena = arenaCreate(1024 * 1024);
    init_symtab();

    int result = yyparse();

    arenaDestroy(&astArena);
    arenaDestroy(&evalArena);

    return result;
}