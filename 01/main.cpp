#include <iostream>
#include "allocator.h"

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

    char *bloc_1 = a.alloc(1);
    for (size_t i = 0; i < 1; i ++) bloc_1[i] = i;

    char *bloc_2 = a.alloc(3);
    for (size_t i = 0; i < 3; i ++) bloc_2[i] = i + 1;

    char *bloc_3= a.alloc(2);
    for (size_t i = 0; i < 2; i ++) bloc_3[i] = i + 4;
    
    if ((int(bloc_1[0]) != 0) || 
        (int(bloc_2[0]) != 1) || (int(bloc_2[1]) != 2) || (int(bloc_2[2]) != 3) ||
        (int(bloc_3[0]) != 4) || (int(bloc_3[1]) != 5)) std::cout << "testWrittingDataInFullMemory had aborted" << std::endl;
}

int main()
{
    //TESTS

    testAllocateBiggerThanMaked();
    testAllocateMemory();
    testAllocateFullMemoryWithAllocManyTimes();
    testWrittingDataInFullMemory();

    std::cout << "Complete!" << std::endl;

    return 0;
}