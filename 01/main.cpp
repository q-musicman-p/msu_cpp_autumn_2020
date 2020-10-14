#include <iostream>
#include "allocator.h"


int main()
{
    
    std::cout << "Complete!" << std::endl;

    return 0;
}

void testAllocateBiggerThanMaked()
{
    Allocator a;
    a.makeAllocator(100);

    char *pointer = a.alloc(120);

    if (pointer != nullptr) std::cout << "testAllocateBiggerThanMaked had aborted" << std::endl;
}

void testAllocateMemory()
{
    Allocator a;
    a.makeAllocator(100);

    char *pointer = a.alloc(50);

    if (pointer == nullptr) std::cout << "testAllocateMemory had aborted" << std::endl;
}

void testAllocateFullMemoryWithAllocManyTimes()
{
    Allocator a;
    a.makeAllocator(100);

    char *p1 = a.alloc(30);
    char *p2 = a.alloc(50);
    char *p3 = a.alloc(20);

    if ((p1 == nullptr) || (p2 == nullptr) || (p3 == nullptr)) std::cout << "testAllocateFullMemoryWithAllocManyTimes had aborted" << std::endl;
}

void testWrittingDataInFullMemory()
{
    Allocator a;
    a.makeAllocator(6);

    char *bloc_1_bytes = a.alloc(1);
    for (size_t i = 0; i < 1; i ++) bloc_1_bytes[i] = i;

    char *bloc_3_bytes = a.alloc(3);
    for (size_t i = 0; i < 3; i ++) bloc_3_bytes[i + 1] = i + 1;

    char *bloc_2_bytes = a.alloc(2);
    for (size_t i = 0; i < 2; i ++) bloc_3_bytes[i + 4] = i + 4;
    
    if ((bloc_1_bytes[0] != 0) || (bloc_3_bytes[0] != 1) || (bloc_3_bytes[1] != 2) || (bloc_3_bytes[2] != 3) ||
        (bloc_2_bytes[0] != 4) || (bloc_2_bytes[1] != 5)) std::cout << "testWrittingDataInFullMemory had aborted" << std::endl;
}