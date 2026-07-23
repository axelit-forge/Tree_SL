cmake_minimum_required(VERSION 3.20)
project(Tree LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(
        include
        common
        ${CMAKE_CURRENT_BINARY_DIR}
)


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


bison_target(Parser src/tree.y ${CMAKE_CURRENT_BINARY_DIR}/tree.tab.c)
flex_target(Scanner src/tree.lx ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
add_flex_bison_dependency(Scanner Parser)


set(SOURCES
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        src/tree_ast.c
        src/tree_errors.c
        src/tree_eval.c
        src/tree_symtab.c
        src/tree_data.c
        common/arena.c
        common/string_view.c
        src/main.c
)

add_executable(tree ${SOURCES})