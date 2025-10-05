#ifndef GEJ_MAX_HEAP_INTERNAL_H
#define GEJ_MAX_HEAP_INTERNAL_H

#include "gej_MaxHeap.h"

typedef struct MaxHeap {
	size_t elementSize;
	void* head;							// pointer to head of heap
	int (*fpCompare)(void* a, void* b);	// pointer to comparison function
	unsigned int numElements;
} MaxHeap;

/* PRIVATE FUNCTION DECLARATIONS */
unsigned int GetParentIndex(unsigned int pos);
unsigned int GetLeftChildIndex(unsigned int pos);
void* GetPointerToElement(MaxHeap* maxHeap, unsigned int pos);
void SwapElements(MaxHeap* h, unsigned int pos1, unsigned int pos2);
// void MaxHeapBubbleUp(MaxHeap* h, unsigned int pos);
void BubbleDown(MaxHeap* h, unsigned int pos);
void PrintElement(MaxHeap* maxHeap, void (*fpPrintElement)(void* element), unsigned int pos);

#endif
