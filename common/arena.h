#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define ARENA_DEFAULT_CAPACITY (1024 * 1024) //1 MB

typedef struct {
 uint8_t* buffer;
 size_t capacidad;
 size_t offset;
} Arena;

/* =========================================================================
 * 1. Gestión del ciclo de vida (Memoria del Sistema)
 * ========================================================================= */
void  arenaInit(Arena *arena, void *bufferExterno, size_t tamBuffer);
Arena arenaCreate(size_t tamBuffer);
void  arenaReset(Arena *arena);
void  arenaDestroy(Arena *arena);

/* =========================================================================
 * 2. Asignación de memoria
 * ========================================================================= */
void *arenaAllocAligned(Arena *arena, size_t size, size_t alineacion);
void *arenaAlloc(Arena *arena, size_t size); // Llama a Aligned usando alignof(void*)
void *arenaAllocZero(Arena *arena, size_t size); // Asigna y hace memset a 0
void *arenaRealloc(Arena *arena, void *oldPtr, size_t oldSize, size_t newSize);

/* =========================================================================
 * 3. Gestión de Ámbitos (Snapshots / Rollback)
 * ========================================================================= */
typedef size_t ArenaMarker;

ArenaMarker arenaSave(Arena *arena);
void        arenaRestore(Arena *arena, ArenaMarker marker);

/* =========================================================================
 * 4. Diagnóstico
 * ========================================================================= */
size_t arenaGetFreeSpace(Arena *arena);


#endif