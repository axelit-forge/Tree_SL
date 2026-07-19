cmake_minimum_required(VERSION 3.20)
project(Tree_SL C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

bison_target(MyParser tree.y ${CMAKE_CURRENT_BINARY_DIR}/tree.tab.c)
flex_target(MyLexer tree.lx ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
add_flex_bison_dependency(MyLexer MyParser)

# Le decimos a CMake que busque los .h tanto en la raíz, en build, como en include/
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tree
        data.c
        data.h
        string.c
        string.h

        src/tree_errors.c
        src/tree_ast.c
        src/tree_symtab.c
        src/tree_eval.c
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyLexer_OUTPUTS}
)

target_link_libraries(tree PRIVATE ${FLEX_LIBRARIES})