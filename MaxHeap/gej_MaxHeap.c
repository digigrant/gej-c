#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gej_MaxHeap_internal.h"

/* PUBLIC FUNCTION IMPLEMENTATIONS */
MaxHeap* MaxHeapCreate(void* head, size_t elementSize, unsigned int numElements, int (*fpCompare)(void* a, void* b)) {
	MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
	if (!maxHeap) { return NULL; }

	maxHeap->head = head;
	maxHeap->elementSize = elementSize;
	maxHeap->numElements = numElements;
	maxHeap->fpCompare = fpCompare;

	// heapify
	unsigned int posLastParent = GetParentIndex((maxHeap->numElements)-1);
	for (int i = posLastParent; i > -1; --i) {
		BubbleDown(maxHeap, i);
	}

	return maxHeap;
}

void MaxHeapFree(MaxHeap** maxHeap) {
	if (!maxHeap || !(*maxHeap)) { return; }
	free(*maxHeap);
	(*maxHeap) = NULL;
}

/*
void MaxHeapInsert(MaxHeap* maxHeap, Node* newNode) {
	++(maxHeap->n);
	memcpy(&(maxHeap->head[maxHeap->n]), newNode, sizeof(Node)); 
	//h->head[h->n] = x;
	MaxHeapBubbleUp(maxHeap, maxHeap->n);
	// MaxHeapPrint(h);
}
*/

void* MaxHeapPop(MaxHeap* maxHeap) {
	if (maxHeap->numElements < 1) {
		printf("Tried to pop from empty queue. Returning NULL.\n");
		return NULL;
	}

	SwapElements(maxHeap, 0, (maxHeap->numElements)-1);	// swap 0 and last element
	--(maxHeap->numElements);
	BubbleDown(maxHeap, 0);

	// this is a pointer to data that is not in the "active" maxHeap anymore
	// (it is still inside the array but outside of head[numElements])
	// it may be invalidated if it is not used right away
	return GetPointerToElement(maxHeap, maxHeap->numElements);
}

void MaxHeapPrint(MaxHeap* maxHeap, void (*fpPrintElement)(void* element)) {
	printf("Printing heap\n");
	PrintElement(maxHeap, fpPrintElement, 0);
}

/* PRIVATE FUNCTION IMPLEMENTATIONS */
unsigned int GetParentIndex(unsigned int pos) {
	return (pos-1) / 2;
}

unsigned int GetLeftChildIndex(unsigned int pos) {
	return (pos*2) + 1;
}

void* GetPointerToElement(MaxHeap* maxHeap, unsigned int pos) {
	return maxHeap->head + (pos * maxHeap->elementSize);
}

/*
void MaxHeapBubbleUp(MaxHeap* maxHeap, unsigned int i) {
	if (MaxHeapGetParentIndex(i) < 1) { return; }

	int compareResult = maxHeap->fpCompare(&(maxHeap->head[i]), &(maxHeap->head[MaxHeapGetParentIndex(i)]));
	if (compareResult > 0) {	// a > b
		MaxHeapSwap(maxHeap, i, MaxHeapGetParentIndex(i));
		MaxHeapBubbleUp(maxHeap, MaxHeapGetParentIndex(i));
	}
}
*/

void SwapElements(MaxHeap* maxHeap, unsigned int pos1, unsigned int pos2) {
	char buffer[maxHeap->elementSize];
	memcpy(&buffer, GetPointerToElement(maxHeap, pos1), maxHeap->elementSize);								// buffer	<- pos1
	memcpy(GetPointerToElement(maxHeap, pos1), GetPointerToElement(maxHeap, pos2), maxHeap->elementSize);	// pos1		<- pos2
	memcpy(GetPointerToElement(maxHeap, pos2), &buffer, maxHeap->elementSize);								// pos2		<- buffer
}

void BubbleDown(MaxHeap* maxHeap, unsigned int pos) {
	unsigned int leftChild = GetLeftChildIndex(pos);
	unsigned int maxIndex = pos;

	// get max value out of pos, left child, right child
	for (int i = 0; i <= 1; ++i) {
		if (leftChild+i < maxHeap->numElements) {
			if (maxHeap->fpCompare(GetPointerToElement(maxHeap, leftChild+i), GetPointerToElement(maxHeap, maxIndex)) > 0) {
				maxIndex = leftChild+i;
			}
		}
	}

	// if pos is not the max value, swap pos and max
	// continue bubbling pos down
	if (maxIndex != pos) {
		SwapElements(maxHeap, pos, maxIndex);
		BubbleDown(maxHeap, maxIndex);
	}
}

void PrintElement(MaxHeap* maxHeap, void (*fpPrintElement)(void* element), unsigned int pos) {
	if (pos >= maxHeap->numElements) { return; }

	unsigned int depth = log2(pos+1);
	for (int i = 0; i < depth; ++i) { printf("\t"); }
	fpPrintElement(GetPointerToElement(maxHeap, pos));

	PrintElement(maxHeap, fpPrintElement, GetLeftChildIndex(pos));
	PrintElement(maxHeap, fpPrintElement, GetLeftChildIndex(pos)+1);
}
