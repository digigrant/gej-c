#include <gtest/gtest.h>

extern "C" {
    #include "gej_MaxHeap_internal.h"
}

int IntCompare(void* a, void* b) {
    return (*(int*)a) - (*(int*)b);
}

TEST(MaxHeapTests, MaxHeapCreate) {
    int array[5] = { 1, 2, 3, 4, 5 };
    MaxHeap* maxHeap = MaxHeapCreate(&array, sizeof(int), 5, IntCompare);
    ASSERT_NE(maxHeap, nullptr);

    MaxHeapFree(&maxHeap);
    ASSERT_EQ(maxHeap, nullptr);
}
