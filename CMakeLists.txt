cmake_minimum_required(VERSION 3.20)
project(Tree_SL C)

# Configuración del estándar de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Buscar Flex y Bison en WSL
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Definir la generación del Parser (Bison)
bison_target(MyParser tree.y ${CMAKE_CURRENT_BINARY_DIR}/tree.tab.c)

# Definir la generación del Lexer (Flex)
flex_target(MyLexer tree.lx ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
add_flex_bison_dependency(MyLexer MyParser)

# Incluir directivas de búsqueda tanto en la raíz como en la carpeta build
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Definir el ejecutable final mezclando tus módulos base y el nuevo ast2.c
add_executable(tree
        controller.c
        data.c
        data.h
        string.c
        string.h
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyLexer_OUTPUTS}
)

# Vincular la librería de Flex
target_link_libraries(tree PRIVATE ${FLEX_LIBRARIES})