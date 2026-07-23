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
%token EOL T_ADD T_KICK T_TAKE
%token T_IF T_ELSE T_ENDIF T_WHILE T_DO T_END T_FORALL T_FORANY
%token T_FN T_ENDFN 
%token T_MAIN T_ENDMAIN T_PESOS_TREE

%token <td> NUM_INT ATOM NUM_DOUBLE  T_BOOL
%token <s> ID

%right '='
%right T_NOT
%left T_AND T_OR
%left T_MAYOR T_MAYOR_IGUAL T_MENOR T_MENOR_IGUAL T_IGUAL T_DISTINTO T_IN T_CONTAINS
%left T_GET
%left T_UNION
%left T_INTER
%left T_DIFF
%left T_FROM 
%left T_CONCAT
%left T_TO
%left '+' '-'
%left '*' '/'
%right T_MENOS_UNARIO

%type <a> exp stm block
%type <a> lit_struct list_exp
%type <sl> list_id

%start tree


%%
tree: 
 T_PESOS_TREE EOL interpreter_tree
| defs main
;

defs:
| defs fn_def
;
main: T_MAIN ':' block T_ENDMAIN { printf("output: "); mostrarData(eval($3)); printf("\nEjecion finalizada\n"); }
;

interpreter_tree:
| interpreter_tree exp    EOL { procesar_expresion($2); }
| interpreter_tree stm    EOL { procesar_expresion($2); }
| interpreter_tree fn_def EOL { printf("Funcion Definida.\n"); arenaReset(&astArena); }
;


block:      { $$ = NULL; }
| stm block { $$ = ($2 == NULL)? $1: newast(NODE_BLOCK, $1, $2, NULL); }
;

stm: exp ';'  { $$ = $1; }
| T_IF '(' exp ')' block T_ENDIF                      { $$ = newflow(NODE_IF,     $3, NULL, $5 , NULL, NULL); }
| T_IF '(' exp ')' block T_ELSE  block T_ENDIF        { $$ = newflow(NODE_IF,     $3, NULL, $5 , $7  , NULL); }

| T_WHILE '(' exp ')' T_DO block T_END                  { $$ = newflow(NODE_WHILE,  $3, NULL, $6 , NULL, NULL); }

| T_FORALL '(' ID T_IN exp '|' exp ')' T_DO stm T_END { $$ = newflow(NODE_FORALL, $7, $5  , $10, NULL, $3  ); }
| T_FORANY '(' ID T_IN exp '|' exp ')' T_DO stm T_END { $$ = newflow(NODE_FORANY, $7, $5  , $10, NULL, $3  ); }
| T_FORALL '(' ID T_IN exp ')' T_DO stm T_END { $$ = newflow(NODE_FORALL, NULL, $5  , $8, NULL, $3  ); }
| T_FORANY '(' ID T_IN exp ')' T_DO stm T_END { $$ = newflow(NODE_FORANY, NULL, $5  , $8, NULL, $3  ); }
;

exp: NUM_INT    { $$ = newast(NODE_INT   , NULL, NULL, $1); }
| ATOM          { $$ = newast(NODE_STR   , NULL, NULL, $1); }
| NUM_DOUBLE    { $$ = newast(NODE_DOUBLE, NULL, NULL, $1); }
| T_BOOL        { $$ = newast(NODE_BOOL  , NULL, NULL, $1); }
| lit_struct    { $$ = $1;}

| exp '+' exp   { $$ = newast('+',$1,$3,NULL); }
| exp '-' exp   { $$ = newast('-',$1,$3,NULL); }
| exp '*' exp   { $$ = newast('*',$1,$3,NULL); }
| exp '/' exp   { $$ = newast('/',$1,$3,NULL); }

| '(' exp ')'   { $$ = $2;}

| '-' exp %prec T_MENOS_UNARIO { $$ = newast(NODE_MENOS_UNARIO, $2, NULL, NULL); }

| '|' exp '|'   { $$ = newast(NODE_MODULO, $2, NULL, NULL); }

| exp T_AND exp         { $$ = newast(NODE_AND        , $1, $3, NULL); }
| exp T_OR exp          { $$ = newast(NODE_OR         , $1, $3, NULL); }
| T_NOT exp             { $$ = newast(NODE_NOT      , NULL, $2, NULL); }

| exp T_MAYOR exp       { $$ = newast(NODE_MAYOR      , $1, $3, NULL); }
| exp T_MENOR exp       { $$ = newast(NODE_MENOR      , $1, $3, NULL); }
| exp T_MAYOR_IGUAL exp { $$ = newast(NODE_MAYOR_IGUAL, $1, $3, NULL); }
| exp T_MENOR_IGUAL exp { $$ = newast(NODE_MENOR_IGUAL, $1, $3, NULL); }
| exp T_DISTINTO exp    { $$ = newast(NODE_DISTINTO   , $1, $3, NULL); }
| exp T_IGUAL exp       { $$ = newast(NODE_IGUAL      , $1, $3, NULL); }

| T_ADD exp T_TO exp    { $$ = newast(NODE_ADD,   $2 , $4, NULL); }
| T_KICK exp T_FROM exp { $$ = newast(NODE_KICK,  $2 , $4, NULL); }
| T_TAKE exp T_FROM exp { $$ = newast(NODE_TAKE,  $2 , $4, NULL); }
| exp T_CONCAT exp      { $$ = newast(NODE_CONCAT,$1 , $3, NULL); }

| exp T_IN exp         { $$ = newast(NODE_IN,       $1, $3, NULL); }
| exp T_CONTAINS exp   { $$ = newast(NODE_CONTAINS, $1, $3, NULL); }

| exp T_UNION exp      { $$ = newast(NODE_UNION,    $1, $3, NULL); }
| exp T_INTER exp      { $$ = newast(NODE_INTER,    $1, $3, NULL); }
| exp T_DIFF exp       { $$ = newast(NODE_DIFF,     $1, $3, NULL); }

| T_GET exp T_FROM exp %prec T_GET { $$ = newast(NODE_GET,      $2, $4, NULL); }

| ID '=' exp            { $$ = newmemory_ast(NODE_ASIGNACION, $1, $3  ); }
| ID                    { $$ = newmemory_ast(NODE_VAR_REF   , $1, NULL); }
| ID '(' list_exp ')'   { $$ = newmemory_ast(NODE_FN_CALL   , $1, $3  ); }
;

lit_struct: '[' ']' { $$ = newast(NODE_LIST, NULL, NULL, createData(NODE_LIST));}
| '[' list_exp ']'  { $$ = newast(NODE_LIST, $2,   NULL, createData(NODE_LIST));}
| '{' '}'           { $$ = newast(NODE_SET,  NULL, NULL, createData(NODE_SET)); }
| '{' list_exp'}'   { $$ = newast(NODE_SET,  $2,   NULL, createData(NODE_SET)); }
;

list_exp: exp       { $$ = $1; }
| exp ',' list_exp  { $$ = newast(NODE_LIST_OF_AST, $1, $3, NULL); }
;
list_id: ID         { $$ = addsym($1, NULL); }
| ID ',' list_id    { $$ = addsym($1, $3  ); }
;

fn_def: T_FN ID '(' list_id ')' ':' stm T_ENDFN { add_definition($2, $4, $7); }
;
%%

