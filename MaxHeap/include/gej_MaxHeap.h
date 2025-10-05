#ifndef GEJ_MAX_HEAP_H
#define GEJ_MAX_HEAP_H

#include <stdlib.h>

/*
maxheap
- doesnt know anything about what the node is
- only knows:
--- where the head is
--- how big each element is
--- how many elements there are
--- which function to use to compare two elements
- handles:
--- heapify (on create)
--- pop element
*/

typedef struct MaxHeap MaxHeap;

MaxHeap* MaxHeapCreate(void* head, size_t elementSize, unsigned int numElements, int (*fpCompare)(void* a, void* b));
void MaxHeapFree(MaxHeap** maxHeap);
//void MaxHeapInsert(MaxHeap* h, Node* x);
void* MaxHeapPop(MaxHeap* maxHeap);
void MaxHeapPrint(MaxHeap* maxHeap, void (*fpPrintElement)(void* element));

#endif
