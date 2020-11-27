#include <iostream>
#include "vector.hpp"
#include <vector>

void constrTest()
{
    Vector<int> v1(10);
    Vector<int> v2(10, -234);
    Vector<double> v5(new double[3] { 1.2, 3, 5.4}, 3);
    Vector<int> v3(v1);
    Vector<int> v4(std::move(v1));
}

void equalOperatorsTest()
{
    Vector<double> v1(new double[3] { 1.2, 3, 5.4}, 3);
    Vector<double> v2(new double[3] { 0, -2.6, 9.24}, 3);
    Vector<double> v3(new double[3] { -6, 2, 1.566}, 3);

    v2 = v1;

    v2 = std::move(v3);
}

int main()
{
    //tests

    //constrTest(); //ok, but need testing with operator[]
    //equalOperatorsTest(); // ?? working but not move-constr, wtf


    
    auto v1 = std::vector<int> {1, 2, 3, 4, 5};
    v1.push_back(5);

    std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;
    
    v1.reserve(7);

    std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;

    

    /*auto v2 = std::vector<int> {1, 2, 3, 1, 2,2};
    v2.push_back(4);

    std::cout << "size = " << v2.size() << "\ncapasity = " << v2.capacity() << std::endl;

    v1 = v2;

    std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;*/

    return 0;
}