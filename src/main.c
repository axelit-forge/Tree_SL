#include <stdio.h>
#include "arena.h"

Arena astArena;
Arena evalArena;
int yyparse(void);

int main(void) {
    astArena  = arenaCreate(1024 * 1024); //1 mb para cada arena
    evalArena = arenaCreate(1024 * 1024);

    printf("Tree-SL Interpreter\n");

    int result = yyparse();

    arenaDestroy(&astArena);
    arenaDestroy(&evalArena);

    return result;
}