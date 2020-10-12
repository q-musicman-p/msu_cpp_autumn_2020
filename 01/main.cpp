#include <iostream>
#include "allocator.h"


int main()
{
    Allocator my_allocator;

    char *p = my_allocator.makeAllocator(15);
    
    p[0] = 7;
    p[3] = 8;

    for (size_t i = 0; i < 15; i++) std::cout << int(p[i]) << " ";
    std::cout << std::endl;

    char *p1 = my_allocator.alloc(5);

    p1[0] = 1;
    p1[2] = 2;
    p1[3] = 4;
    p1[4] = 6;

    for (size_t i = 0; i < 15; i++) std::cout << int(p[i]) << " ";
    std::cout << std::endl;

    char *p2 = my_allocator.alloc(7);
    p2[0]=45;
    p2[6] = 1;

    for (size_t i = 0; i < 15; i++) std::cout << int(p[i]) << " ";
    std::cout << std::endl;

    char *p3 = my_allocator.alloc(3);
    p3[2] = 99;

    for (size_t i = 0; i < 15; i++) std::cout << int(p[i]) << " ";
    std::cout << std::endl;

    my_allocator.reset();

    char *p4 = my_allocator.alloc(12);
    p4[0] = 84;
    p4[11] = 34;

    for (size_t i = 0; i < 15; i++) std::cout << int(p[i]) << " ";
    std::cout << std::endl;

    return 0;
}