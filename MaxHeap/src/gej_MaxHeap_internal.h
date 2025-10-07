#ifndef GEJ_MAX_HEAP_INTERNAL_H
#define GEJ_MAX_HEAP_INTERNAL_H

#include "gej_MaxHeap.h"

unsigned int GetParentIndex(unsigned int pos);
unsigned int GetLeftChildIndex(unsigned int pos);
void* GetPointerToElement(void* array, size_t elementSize, unsigned int numElements, unsigned int pos);
void SwapElements(void* array, size_t elementSize, unsigned int numElements, unsigned int pos1, unsigned int pos2);
void BubbleDown(void* array, size_t elementSize, unsigned int numElements, int (*fpCompare)(void const * a, void const * b), unsigned int pos);
void PrintHeapElement(void* array, size_t elementSize, unsigned int numElements, void (*fpPrintElement)(void const* element), unsigned int pos);

#endif
