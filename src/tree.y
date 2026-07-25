%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "tree_data.h"
    #include "tree_nodetypes.h"
    #include "tree_errors.h"
    #include "tree_ast.h"
    #include "tree_symtab.h"
    #include "tree_eval.h"
    #include "arena.h"

    int yylex();
    int yyerror(char* s);
    extern Arena astArena;
    extern Arena evalArena;


    void procesar_expresion(struct ast* arbol) {
        if (!arbol) return;
        tData res = eval(arbol);
        printf("=> ");
        mostrarData(res);
        printf("\n");

        arenaReset(&astArena);
    }
%}
%union{
    struct ast* a;
    struct symbol* s;
    tData td;
    struct symlist* sl;
}
%token EOL ADD KICK TAKE
%token IF ELSE ENDIF WHILE DO END FORALL FORANY
%token MAIN ENDMAIN PESOS_TREE
%token PRINT
%token FUNCTION
%token RETURN

%token <td> NUM_INT ATOM NUM_DOUBLE BOOL
%token <s> ID

%right '='
%right NOT
%left AND OR
%left MAYOR MAYOR_IGUAL MENOR MENOR_IGUAL IGUAL DISTINTO IN CONTAINS
%left GET
%left UNION
%left INTER
%left DIFF
%left FROM
%left CONCAT
%left TO
%left '+' '-'
%left '*' '/' '%'
%right MENOS_UNARIO

%type <a> exp stm block fn_def
%type <a> lit_struct list_exp
%type <sl> list_id

%start tree


%%
tree:
  PESOS_TREE { printf("Tree-SL Interpreter\n"); } opt_eol interpreter_tree
| defs main
;
defs:
| defs fn_def opt_eol
;

main: MAIN ':' opt_eol block ENDMAIN {
    printf("--- INICIO DE EJECUCIÓN ---\n");
    if ($4 != NULL) {
        mostrarData(eval($4));
    }
    printf("\nEjecución finalizada con éxito.\n");
}
;

interpreter_tree:
| interpreter_tree     EOL {                      }
| interpreter_tree exp EOL { procesar_expresion($2); }
| interpreter_tree stm EOL { procesar_expresion($2); }
| interpreter_tree fn_def EOL { printf("Función definida correctamente.\n"); }
;

fn_def: FUNCTION ID '(' list_id ')' ':' opt_eol block END { add_definition($2, $4, $8); }
| FUNCTION ID '(' ')' ':' opt_eol block END               { add_definition($2, NULL, $7); }
;

opt_eol:
| opt_eol EOL
;

block: { $$ = NULL; }
| stm opt_eol block { $$ = ($3 == NULL) ? $1 : newast(NODE_BLOCK, $1, $3, NULL); }
;

stm: exp ';'                                      { $$ = $1; }
| RETURN exp ';'                                  { $$ = newast(NODE_RETURN, $2, NULL, NULL); }
| RETURN ';'                                      { $$ = newast(NODE_RETURN, NULL, NULL, NULL); }
| PRINT exp ';'                                   { $$ = newast(NODE_PRINT, $2, NULL, NULL); }
| PRINT '(' exp ')' ';'                           { $$ = newast(NODE_PRINT, $3, NULL, NULL); }
| IF '(' exp ')' block ENDIF                    { $$ = newflow(NODE_IF,     $3, NULL, $5,  NULL, NULL); }
| IF '(' exp ')' block ELSE block ENDIF         { $$ = newflow(NODE_IF,     $3, NULL, $5,  $7,   NULL); }

| WHILE '(' exp ')' DO block END                { $$ = newflow(NODE_WHILE,  $3, NULL, $6,  NULL, NULL); }

| FORALL '(' ID IN exp '|' exp ')' DO block END { $$ = newflow(NODE_FORALL, $7, $5,   $10, NULL, $3  ); }
| FORANY '(' ID IN exp '|' exp ')' DO block END { $$ = newflow(NODE_FORANY, $7, $5,   $10, NULL, $3  ); }
| FORALL '(' ID IN exp ')' DO block END         { $$ = newflow(NODE_FORALL, NULL, $5, $8,  NULL, $3  ); }
| FORANY '(' ID IN exp ')' DO block END         { $$ = newflow(NODE_FORANY, NULL, $5, $8,  NULL, $3  ); }
;

exp: NUM_INT    { $$ = newast(NODE_INT   , NULL, NULL, $1); }
| ATOM          { $$ = newast(NODE_STR   , NULL, NULL, $1); }
| NUM_DOUBLE    { $$ = newast(NODE_DOUBLE, NULL, NULL, $1); }
| BOOL          { $$ = newast(NODE_BOOL  , NULL, NULL, $1); }
| lit_struct    { $$ = $1; }

| exp '+' exp   { $$ = newast('+',$1,$3,NULL); }
| exp '-' exp   { $$ = newast('-',$1,$3,NULL); }
| exp '*' exp   { $$ = newast('*',$1,$3,NULL); }
| exp '/' exp   { $$ = newast('/',$1,$3,NULL); }
| exp '%' exp   { $$ = newast('%',$1,$3,NULL); }

| '(' exp ')'   { $$ = $2; }

| '-' exp %prec MENOS_UNARIO { $$ = newast(NODE_MENOS_UNARIO, $2, NULL, NULL); }

| '|' exp '|'   { $$ = newast(NODE_MODULO, $2, NULL, NULL); }

| exp AND exp         { $$ = newast(NODE_AND        , $1, $3, NULL); }
| exp OR exp          { $$ = newast(NODE_OR         , $1, $3, NULL); }
| NOT exp             { $$ = newast(NODE_NOT      , NULL, $2, NULL); }

| exp MAYOR exp       { $$ = newast(NODE_MAYOR      , $1, $3, NULL); }
| exp MENOR exp       { $$ = newast(NODE_MENOR      , $1, $3, NULL); }
| exp MAYOR_IGUAL exp { $$ = newast(NODE_MAYOR_IGUAL, $1, $3, NULL); }
| exp MENOR_IGUAL exp { $$ = newast(NODE_MENOR_IGUAL, $1, $3, NULL); }
| exp DISTINTO exp    { $$ = newast(NODE_DISTINTO   , $1, $3, NULL); }
| exp IGUAL exp       { $$ = newast(NODE_IGUAL      , $1, $3, NULL); }

| ADD exp TO exp    { $$ = newast(NODE_ADD,   $2, $4, NULL); }
| KICK exp FROM exp { $$ = newast(NODE_KICK,  $2, $4, NULL); }
| TAKE exp FROM exp { $$ = newast(NODE_TAKE,  $2, $4, NULL); }
| exp CONCAT exp    { $$ = newast(NODE_CONCAT,$1, $3, NULL); }

| exp IN exp        { $$ = newast(NODE_IN,       $1, $3, NULL); }
| exp CONTAINS exp  { $$ = newast(NODE_CONTAINS, $1, $3, NULL); }

| exp UNION exp     { $$ = newast(NODE_UNION,    $1, $3, NULL); }
| exp INTER exp     { $$ = newast(NODE_INTER,    $1, $3, NULL); }
| exp DIFF exp      { $$ = newast(NODE_DIFF,     $1, $3, NULL); }

| GET exp FROM exp %prec GET { $$ = newast(NODE_GET, $2, $4, NULL); }

| ID '=' exp          { $$ = newmemory_ast(NODE_ASIGNACION, $1, $3  ); }
| ID                  { $$ = newmemory_ast(NODE_VAR_REF   , $1, NULL); }
| ID '(' list_exp ')' { $$ = newmemory_ast(NODE_FN_CALL   , $1, $3  ); }
| ID '(' ')'          { $$ = newmemory_ast(NODE_FN_CALL   , $1, NULL); }
;

lit_struct: '[' ']' { $$ = newast(NODE_LIST, NULL, NULL, createData(NODE_LIST)); }
| '[' list_exp ']'  { $$ = newast(NODE_LIST, $2,   NULL, createData(NODE_LIST)); }
| '{' '}'           { $$ = newast(NODE_SET,  NULL, NULL, createData(NODE_SET)); }
| '{' list_exp '}'  { $$ = newast(NODE_SET,  $2,   NULL, createData(NODE_SET)); }
;

list_exp: exp       { $$ = $1; }
| exp ',' list_exp  { $$ = newast(NODE_LIST_OF_AST, $1, $3, NULL); }
;

list_id: ID         { $$ = addsym($1, NULL); }
| ID ',' list_id    { $$ = addsym($1, $3); }
;

%%