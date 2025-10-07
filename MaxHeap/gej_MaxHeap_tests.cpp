#include <gtest/gtest.h>
#include <stdio.h>

extern "C" {
    #include "gej_MaxHeap_internal.h"
}

struct CustomNode {
    float key = 0.0f;
    int value = 0;
};

int CompareInts(void const * a, void const * b) {
    return (*(int*)a) - (*(int*)b);
}

const float EPSILON = 0.0001f;
int CompareFloats(void const * a, void const * b) {
    float diff = (*(float*)a) - (*(float*)b);

    if (abs(diff) < EPSILON) { return 0; }
    if (diff > 0.0f) { return 1; }
    return -1;
}

int CompareCustom(void const * a, void const * b) {
    CustomNode cn_a = (*(CustomNode*)a);
    CustomNode cn_b = (*(CustomNode*)b);
    return CompareFloats(&(cn_a.key), &(cn_b.key));
}

void PrintInt(void const * i) {
    std::cout << (*(int*)i) << std::endl;
}

void PrintCustom(void const * i) {
    CustomNode* cn = (CustomNode*)i;
    std::cout << "( " << cn->key << " | " << cn->value << " )" << std::endl;
}

bool ValidateMaxHeap(void* array, size_t elementSize, unsigned int numElements, int (*fpCompare)(void const * a, void const * b), unsigned int pos) {
    if (pos >= numElements) { return true; }

    unsigned int leftChild = GetLeftChildIndex(pos);
    unsigned int rightChild = leftChild+1;

    // validate current element against children
    if (leftChild < numElements &&
        fpCompare(
            GetPointerToElement(array, elementSize, numElements, pos),
            GetPointerToElement(array, elementSize, numElements, leftChild)) < 0) {
        return false;
    }
    if (rightChild < numElements &&
        fpCompare(
            GetPointerToElement(array, elementSize, numElements, pos),
            GetPointerToElement(array, elementSize, numElements, rightChild)) < 0) {
        return false;
    }

    // validate child heaps
    if (!ValidateMaxHeap(array, elementSize, numElements, fpCompare, leftChild)) { return false; }
    if (!ValidateMaxHeap(array, elementSize, numElements, fpCompare, rightChild)) { return false; }

    return true;
}

TEST(UtilityTests, CompareInts) {
    int array[2] = { 3, 7 };
    ASSERT_LT(CompareInts(&(array[0]), &(array[1])), 0);
    ASSERT_GT(CompareInts(&(array[1]), &(array[0])), 0);
    ASSERT_EQ(CompareInts(&(array[0]), &(array[0])), 0);
}

TEST(UtilityTests, CompareFloats) {
    float array[2] = { 2.1f, 5.5f };
    ASSERT_LT(CompareFloats(&(array[0]), &(array[1])), 0);
    ASSERT_GT(CompareFloats(&(array[1]), &(array[0])), 0);
    ASSERT_EQ(CompareFloats(&(array[0]), &(array[0])), 0);
}

TEST(UtilityTests, CompareCustom) {
    CustomNode array[2] = {
        { -3.8f, 1 },
        { 2.7f, -4 }
    };

    ASSERT_LT(CompareCustom(&(array[0]), &(array[1])), 0);
    ASSERT_GT(CompareCustom(&(array[1]), &(array[0])), 0);
    ASSERT_EQ(CompareCustom(&(array[0]), &(array[0])), 0);
}

TEST(UtilityTests, ValidateHeap) {

    int array1[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    unsigned int numElements = 10;
    ASSERT_TRUE(ValidateMaxHeap(array1, sizeof(int), numElements, CompareInts, 0));

    int array2[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 10 };
    numElements = 10;
    ASSERT_FALSE(ValidateMaxHeap(array2, sizeof(int), numElements, CompareInts, 0));
}

TEST(MaxHeapTests, HeapifyInt) {
    int array[5] = { 1, 2, 3, 4, 5 };
    unsigned int numElements = 5;
    Heapify(array, sizeof(int), numElements, CompareInts);
    ASSERT_TRUE(ValidateMaxHeap(array, sizeof(int), numElements, CompareInts, 0));
}

TEST(MaxHeapTests, PopInt) {
    int array[5] = { 1, 2, 3, 4, 5 };
    unsigned int numElements = 5;
    Heapify(array, sizeof(int), numElements, CompareInts);
    ASSERT_TRUE(ValidateMaxHeap(array, sizeof(int), numElements, CompareInts, 0));
    
    int* popped = (int*)HeapPop(array, sizeof(int), &numElements, CompareInts);
    ASSERT_EQ(*popped, 5);
    ASSERT_EQ(numElements, 4);
    ASSERT_TRUE(ValidateMaxHeap(array, sizeof(int), numElements, CompareInts, 0));

    HeapPrint(array, sizeof(int), numElements, PrintInt);
}

TEST(MaxHeapTests, HeapifyAndPopCustom) {
    CustomNode array[5] = {
        { -0.5f,    11 },
        {  0.4f,    21 },
        { -0.3f,    41 },
        {  0.2f,   -61 },
        { -0.1f,   -91 }
    };
    unsigned int numElements = 5;

    Heapify(array, sizeof(CustomNode), numElements, CompareCustom);
    ASSERT_TRUE(ValidateMaxHeap(array, sizeof(CustomNode), numElements, CompareCustom, 0));
    
    CustomNode* popped = (CustomNode*)HeapPop(array, sizeof(CustomNode), &numElements, CompareCustom);
    ASSERT_FLOAT_EQ(popped->key, 0.4);
    ASSERT_EQ(popped->value, 21);
    ASSERT_EQ(numElements, 4);
    ASSERT_TRUE(ValidateMaxHeap(array, sizeof(CustomNode), numElements, CompareCustom, 0));
}
