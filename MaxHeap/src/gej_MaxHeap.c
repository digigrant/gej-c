#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gej_MaxHeap_internal.h"

/* PUBLIC FUNCTION IMPLEMENTATIONS */

void Heapify(void* array, size_t elementSize, unsigned int numElements, int (*fpCompare)(void const * a, void const * b)) {
	// get last parent node
	unsigned int posLastParent = GetParentIndex((numElements)-1);
	// bubble all parents down in reverse order
	for (int i = posLastParent; i > -1; --i) {
		BubbleDown(array, elementSize, numElements, fpCompare, i);
	}
}

// NOTE: Automatically decrements numElements after popping
void* HeapPop(void* array, size_t elementSize, unsigned int* numElements, int (*fpCompare)(void const * a, void const * b)) {
	if ((*numElements) < 1) {
		printf("Tried to pop from empty queue. Returning NULL.\n");
		return NULL;
	}

	SwapElements(array, elementSize, (*numElements), 0, (*numElements)-1);	// swap 0 and last element
	--(*numElements);
	BubbleDown(array, elementSize, (*numElements), fpCompare, 0);

	// this is a pointer to data that is not in the "active" maxHeap anymore
	// (it is still inside the array but outside of head[numElements])
	// it may be invalidated if it is not used right away
	return GetPointerToElement(array, elementSize, (*numElements)+1, (*numElements));
}

void HeapPrint(void* array, size_t elementSize, unsigned int numElements, void (*fpPrintElement)(void const * element)) {
	printf("Printing heap\n");
	PrintHeapElement(array, elementSize, numElements, fpPrintElement, 0);
}

/* PRIVATE FUNCTION IMPLEMENTATIONS */
// (pos-1)/2
unsigned int GetParentIndex(unsigned int pos) {
	return (pos-1) / 2;
}

// (pos*2)+1
unsigned int GetLeftChildIndex(unsigned int pos) {
	return (pos*2) + 1;
}

void* GetPointerToElement(void* array, size_t elementSize, unsigned int numElements, unsigned int pos) {
	if (pos >= numElements) {	// this seems like a good idea
		printf("Tried to get the pointer to an element outside of the max heap: index %d (numElements = %d)\n", pos, numElements);
		return NULL;
	}
	return array + (pos * elementSize);
}

void SwapElements(void* array, size_t elementSize, unsigned int numElements, unsigned int pos1, unsigned int pos2) {
	char buffer[elementSize];
	// 1. buffer	<- pos1
	// 2. pos1		<- pos2
	// 3. pos2		<- buffer
	memcpy(&buffer, GetPointerToElement(array, elementSize, numElements, pos1), elementSize);
	memcpy(GetPointerToElement(array, elementSize, numElements, pos1), GetPointerToElement(array, elementSize, numElements, pos2), elementSize);
	memcpy(GetPointerToElement(array, elementSize, numElements, pos2), &buffer, elementSize);
}

void BubbleDown(void* array, size_t elementSize, unsigned int numElements, int (*fpCompare)(void const * a, void const * b), unsigned int pos) {
	unsigned int leftChild = GetLeftChildIndex(pos);
	unsigned int maxIndex = pos;

	// get max value out of pos, left child, right child
	for (int i = 0; i <= 1; ++i) {
		if (leftChild+i < numElements) {
			if (fpCompare(GetPointerToElement(array, elementSize, numElements, leftChild+i), GetPointerToElement(array, elementSize, numElements, maxIndex)) > 0) {
				maxIndex = leftChild+i;
			}
		}
	}

	// if pos is not the max value, swap pos and max
	// continue bubbling pos down
	if (maxIndex != pos) {
		SwapElements(array, elementSize, numElements, pos, maxIndex);
		BubbleDown(array, elementSize, numElements, fpCompare, maxIndex);
	}
}

void PrintHeapElement(void * array, size_t elementSize, unsigned int numElements, void (*fpPrintElement)(void const* element), unsigned int pos) {
	if (pos >= numElements) { return; }

	unsigned int depth = log2(pos+1);
	for (int i = 0; i < depth; ++i) { printf("\t"); }
	fpPrintElement(GetPointerToElement(array, elementSize, numElements, pos));

	PrintHeapElement(array, elementSize, numElements, fpPrintElement, GetLeftChildIndex(pos));
	PrintHeapElement(array, elementSize, numElements, fpPrintElement, GetLeftChildIndex(pos)+1);
}
