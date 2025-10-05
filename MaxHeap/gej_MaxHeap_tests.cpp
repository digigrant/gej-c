#include <gtest/gtest.h>
#include <stdio.h>

extern "C" {
    #include "gej_MaxHeap_internal.h"
}

struct CustomNode {
    float key = 0.0f;
    int value = 0;
};

int CompareInts(void* a, void* b) {
    return (*(int*)a) - (*(int*)b);
}

const float EPSILON = 0.0001f;
int CompareFloats(void* a, void* b) {
    float diff = (*(float*)a) - (*(float*)b);

    if (abs(diff) < EPSILON) { return 0; }
    if (diff > 0.0f) { return 1; }
    return -1;
}

int CompareCustom(void* a, void* b) {
    CustomNode cn_a = (*(CustomNode*)a);
    CustomNode cn_b = (*(CustomNode*)b);
    return CompareFloats(&(cn_a.key), &(cn_b.key));
}

void PrintInt(void* i) {
    std::cout << (*(int*)i) << std::endl;
}

bool ValidateMaxHeap(MaxHeap* maxHeap, unsigned int pos) {
    if (pos >= maxHeap->numElements) { return true; }

    unsigned int leftChild = GetLeftChildIndex(pos);
    unsigned int rightChild = leftChild+1;

    // validate current element against children
    if (leftChild < maxHeap->numElements &&
        maxHeap->fpCompare(GetPointerToElement(maxHeap, pos), GetPointerToElement(maxHeap, leftChild)) < 0) {
        return false;
    }
    if (rightChild < maxHeap->numElements &&
        maxHeap->fpCompare(GetPointerToElement(maxHeap, pos), GetPointerToElement(maxHeap, rightChild)) < 0) {
        return false;
    }

    // validate child heaps
    if (!ValidateMaxHeap(maxHeap, leftChild)) { return false; }
    if (!ValidateMaxHeap(maxHeap, rightChild)) { return false; }

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

TEST(UtilityTests, ValidateMaxHeap) {
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    ASSERT_NE(maxHeap, nullptr);

    int array1[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    maxHeap->head = &array1;
    maxHeap->elementSize = sizeof(int);
    maxHeap->numElements = 10;
    maxHeap->fpCompare = CompareInts;
    ASSERT_TRUE(ValidateMaxHeap(maxHeap, 0));

    int array2[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 10 };
    maxHeap->head = &array2;
    ASSERT_FALSE(ValidateMaxHeap(maxHeap, 0));
}

TEST(MaxHeapTests, CreateAndFree) {
    int array[5] = { 1, 2, 3, 4, 5 };
    MaxHeap* maxHeap = MaxHeapCreate(&array, sizeof(int), 5, CompareInts);
    ASSERT_NE(maxHeap, nullptr);

    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
}

TEST(MaxHeapTests, EmptyFreeAndDoubleFree) {
    int array[5] = { 1, 2, 3, 4, 5 };
    MaxHeap* maxHeap = nullptr;

    // Empty free
    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
    
    // Double free
    maxHeap = MaxHeapCreate(&array, sizeof(int), 5, CompareInts);
    ASSERT_NE(maxHeap, nullptr);
    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
}

TEST(MaxHeapTests, Heapify) {
    int array[5] = { 1, 2, 3, 4, 5 };
    MaxHeap* maxHeap = MaxHeapCreate(&array, sizeof(int), 5, CompareInts);
    ASSERT_NE(maxHeap, nullptr);

    ASSERT_TRUE(ValidateMaxHeap(maxHeap, 0));

    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
}

TEST(MaxHeapTests, Pop) {
    int array[5] = { 1, 2, 3, 4, 5 };
    MaxHeap* maxHeap = MaxHeapCreate(&array, sizeof(int), 5, CompareInts);
    ASSERT_NE(maxHeap, nullptr);
    
    int* popped = (int*)MaxHeapPop(maxHeap);
    ASSERT_EQ(*popped, 5);
    ASSERT_EQ(maxHeap->numElements, 4);
    ASSERT_TRUE(ValidateMaxHeap(maxHeap, 0));

    MaxHeapPrint(maxHeap, PrintInt);

    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
}
