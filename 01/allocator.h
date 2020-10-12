#include <iostream>

class Allocator
{
public:
    Allocator(): order(nullptr), memory(nullptr), allocation_size(0) {}

    char* makeAllocator(size_t size)
    {
        if (allocation_size > 0)
        {
            std::cout << "Memory has been allocated yet!" << std::endl;
            throw;
        }


        memory = new char[size];
        if (!memory)
        {
            throw std::bad_alloc::exception();
        }

        order = memory;
        allocation_size = size;

        return memory;
    }

    char* alloc(size_t size)
    {
        if ((order - memory) + size > allocation_size) return nullptr;

        char *begin = order;
        order += size;

        return begin;
    }

    void reset()
    {
        order = memory;
    }

    ~Allocator()
    {
        delete[] memory;
    }
private:
    char* order;
    char* memory;
    size_t allocation_size;
};