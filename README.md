# Tree-SL Interpreter

Este proyecto consiste en el rediseño arquitectónico, modernización y optimización de bajo nivel de **Tree-SL**, un intérprete basado en recorrido de Árbol de Sintaxis Abstracta (*AST-walking interpreter*), especializado en el procesamiento nativo de colecciones (Listas) y operaciones de teoría de conjuntos (Sets).

El desarrollo original fue refactorizado para mitigar la deuda técnica acumulada durante su prototipo pedagógico inicial, transitando de una estructura monolítica hacia un diseño modular con interfaces limpias (`include/`), implementaciones desacopladas (`src/`), vistas contiguas de memoria (`common/`) y pruebas automatizadas (`tests/`).

---

##  Características y Optimizaciones

* **Arquitectura Modular:** Desacoplamiento estricto entre el análisis léxico/sintáctico (Flex & Bison), la construcción de nodos del AST (`tree_ast`), la evaluación en tiempo de ejecución (`tree_eval`) y la tabla de símbolos (`tree_symtab`).
* **Arena Allocator & StringView:** Asignador de memoria lineal contiguo para nodos del AST y entornos de evaluación, reduciendo llamadas a `malloc/free`, garantizando localidad espacial en caché y descarte en $\mathcal{O}(1)$.
* **Destructuración de Tuplas:** Soporte nativo para desempaquetado de colecciones mediante la sintaxis `let A => (a, b, c);` con validación estricta de cardinalidad en runtime.
* **Manejo Pragmático de Ámbitos y Recursión:** Soporte para funciones anidadas, pasaje de parámetros y recursión profunda mediante el mecanismo *Save-and-Restore* sobre la tabla de símbolos.
* **Diagnóstico Tipificado:** Subsistema centralizado (`tree_notify`) con taxonomía de errores por criticidad (advertencias y errores fatales) eliminando identificadores numéricos ambiguos.

---

##  Tecnologías Utilizadas

* **C11** (Estándar de implementación de bajo nivel)
* **Flex 2.6+** (Generador de analizador léxico)
* **Bison 3.8+** (Generador de analizador sintáctico LALR)
* **CMake 3.16+** (Sistema de construcción y automatización)
* **Make / Ninja** (Generadores de compilación)

---

##  Compilación

Para compilar el proyecto en sistemas basados en Linux o WSL2:

```bash
# 1. Crear el directorio de construcción
mkdir -p build && cd build

# 2. Configurar el proyecto con CMake
cmake ..

# 3. Compilar el binario ejecutable
cmake --build . -j $(nproc)
```

El binario ejecutable tree se generará en el directorio de compilación.

## Ejecución de Tests y Scripts

```bash
# Ejecutar prueba de recursión matemática (Factorial)
 ./cmake-build-debug/tree < tests/afnd2afd.tsl
 ./cmake-build-debug/tree < tests/scripttest.tsl
```

## Estructura
```
Tree-SL/
├── include/       # Cabeceras 
├── src/           # Código fuente C y especificaciones (.lx, .y)
├── common/        # Utilidades 
├── tests/         # Casos de prueba y documentacion
└── CMakeLists.txt # Configuración de compilación automatizada