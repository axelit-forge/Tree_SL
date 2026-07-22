#include "arena.h"

// 1. Gestión del ciclo de vida (Memoria del Sistema)
void  arenaInit(Arena *arena, void *bufferExterno, size_t tamBuffer) {
    if (!arena) return;
    arena->buffer = bufferExterno;
    arena->capacidad = tamBuffer;
    arena->offset = 0;
}
Arena arenaCreate(size_t tamBuffer) {
    Arena arena = {0};
    void* buffer = malloc(tamBuffer);

    if (buffer != NULL) {
        arenaInit(&arena, buffer, tamBuffer);
    }
    return arena;
}

void  arenaReset(Arena *arena) {
    if (arena) arena->offset = 0;
}
void  arenaDestroy(Arena *arena) {
    if (!arena) return;
    if (arena->buffer != NULL) {
        free(arena->buffer);
        arena->buffer = NULL;
    }
    arena->capacidad = 0;
    arena->offset = 0;
}

// 2. Asignación de memoria
static void* avanzarBytes(void* punteroBase, size_t bytesMover) {
    char* temporal = punteroBase;
    return (void*)(temporal + bytesMover);
}

static void* alinearPuntero(void* direccionActual, size_t alineacion) {
    uintptr_t direccion = (uintptr_t)direccionActual;
    return (void*)((direccion + (alineacion - 1)) & ~(alineacion - 1));
}

void *arenaAllocAligned(Arena *arena, size_t size, size_t alineacion) {

    if (!arena || !arena->buffer || size == 0) return NULL;

    if (alineacion == 0 || (alineacion & (alineacion - 1)) != 0) {
        alineacion = sizeof(void*);
    }

    void* dirActual = avanzarBytes(arena->buffer, arena->offset);
    void* dirAlineada = alinearPuntero(dirActual, alineacion);
    void* dirFinal = avanzarBytes(dirAlineada, size);
    void* limiteArena = avanzarBytes(arena->buffer, arena->capacidad);

    if (dirFinal > limiteArena) {
        return NULL;
    }
    arena->offset = (uint8_t*)dirFinal - arena->buffer;
    return dirAlineada;
}
void *arenaAlloc(Arena *arena, size_t size){
    return arenaAllocAligned(arena, size, sizeof(void*));
}
void *arenaAllocZero(Arena *arena, size_t size) {
    void* ptr = arenaAlloc(arena, size);
    if (ptr != NULL) {
        uint8_t* p = ptr;
        for (size_t i = 0; i < size; i++) p[i] = 0;
    }
    return ptr;
}

void *arenaRealloc(Arena *arena, void *oldPtr, size_t oldSize, size_t newSize) {
    if (oldPtr == NULL) {
        return arenaAlloc(arena, newSize);
    }
    if (newSize <= oldSize) {
        return oldPtr;
    }
    uint8_t* finOldPtr = (uint8_t*)oldPtr + oldSize;
    uint8_t* posicionActualArena = arena->buffer + arena->offset;
    if (finOldPtr == posicionActualArena) {
        size_t bytesExtra = newSize - oldSize;
        if (arena->offset + bytesExtra <= arena->capacidad) {
            arena->offset += bytesExtra;
            return oldPtr;
        }
        return NULL;
    }
    void* nuevoBloque = arenaAlloc(arena, newSize);
    if (nuevoBloque != NULL) {
        uint8_t* d = nuevoBloque;
        const uint8_t* s = oldPtr;
        for (size_t i = 0; i < oldSize; i++) d[i] = s[i];
    }
    return nuevoBloque;
}

// 3. Gestión de Ámbitos (Snapshots / Rollback)
typedef size_t ArenaMarker;

ArenaMarker arenaSave(Arena *arena) {
    return arena->offset;
}
void arenaRestore(Arena *arena, ArenaMarker marker) {
    if (arena && marker <= arena->capacidad) {
        arena->offset = marker;
    }
}

// 4. Diagnóstico
size_t arenaGetFreeSpace(Arena *arena) {
    if (arena == NULL || arena->offset >= arena->capacidad) {
        return 0;
    }
    return arena->capacidad - arena->offset;
}
