#include <iostream>

class Allocator
{
public:
    Allocator(): order(nullptr), memory(nullptr), allocation_size(0) {}

    void makeAllocator(size_t size)
    {
        if (allocation_size > 0)
        {
            std::cout << "Memory has been allocated yet!" << std::endl;
            return;
        }

        memory = new char[size];
        if (!memory)
        {   
            // if memory had not allocated
            throw std::bad_alloc::exception();
        }

        order = memory;
        allocation_size = size;
    }

    char* alloc(size_t size)
    {
        if ((order - memory) + size > allocation_size)
        {
            return nullptr;
        }

        char *begin = order;
        order += size;

        return begin;
    }

    void reset()
    {
        // move order pointer to begin
        order = memory;
    }

    ~Allocator()
    {
        delete[] memory;
    }
private:
    char* order; //pointer to current memory block
    char* memory; //pointer to begin memory block
    size_t allocation_size;
};