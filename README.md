# Tree-SL Interpreter

Este proyecto consiste en el rediseño arquitectónico y la refactorización profunda de un intérprete de lenguaje de programación de una sola pasada (*single-pass*), especializado en el procesamiento nativo de colecciones (Listas) y teoría de conjuntos (Sets).

El desarrollo original fue optimizado para mitigar la deuda técnica acumulada durante su fase de prototipado rápido, migrando de una estructura monolítica hacia un diseño altamente modularizado basado en la separación estricta de interfaces (`include/`) e implementaciones (`src/`).

##  Objetivos de la Refactorización Académica

* **Modularización del AST:** Desacoplamiento de las factorías de construcción de nodos sintácticos de la lógica del despachador de evaluación semántica.
* **Sistema de Diagnóstico por Rangos:** Implementación de un subsistema de manejo de errores correlativo y estandarizado, eliminando códigos numéricos mágicos.
* **Gestión Eficiente de Memoria (Trabajo en Progreso):** Transición de asignaciones dinámicas individuales (`malloc`) hacia un asignador lineal optimizado (*Arena Allocator*) para mitigar fugas de memoria en nodos temporales.
* **Entornos Anidados y Ámbito (Trabajo en Progreso):** Implementación de una pila de contextos dinámicos (*Scope Stack*) para soportar variables locales y recursividad real en funciones.

##  Tecnologías Utilizadas

* **C11** (Estándar de desarrollo)
* **Flex** (Generador de analizador léxico)
* **Bison** (Generador de analizador sintáctico)
* **CMake** (Sistema de construcción y automatización)