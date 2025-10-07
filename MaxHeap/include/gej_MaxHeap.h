#ifndef GEJ_MAX_HEAP_H
#define GEJ_MAX_HEAP_H

#include <stddef.h>

void Heapify(void* array, size_t elementSize, unsigned int numElements, int (*fpCompare)(void const * a, void const * b));
void* HeapPop(void* array, size_t elementSize, unsigned int* numElements, int (*fpCompare)(void const * a, void const * b));
void HeapPrint(void* array, size_t elementSize, unsigned int numElements, void (*fpPrintElement)(void const * element));

#endif
