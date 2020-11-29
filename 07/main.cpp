#include <iostream>
#include "vector.hpp"
#include <vector>

template <class T>
void show(Vector<T>& v)
{
    std::cout << "size = " << v.size() << ", capacity = " << v.capacity() << std::endl;
    for (auto i = v.begin(); i != v.end(); ++i)
    {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
}

void iteratorTest()
{
    Vector<double> v { 1.2, 3, 5.4, 1};

    for (auto i = v.begin(); i != v.end(); ++i)
    {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;

    for (auto i = v.rbegin(); i != v.rend(); ++i)
    {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
}

void constrTest()
{
    Vector<int> v1(10);
    show(v1);

    Vector<int> v2(10, -234);
    show(v2);
    
    Vector<int> v3(v2);
    show(v3);

    Vector<int> v5 { 1, 3, 5 };
    Vector<int> v4(std::move(v5));
    show(v4);    
}

void equalOperatorsTest()
{
    Vector<double> v1 { 1.2, 3, 5.4 };
    Vector<double> v2 { 0, -2.6, 9.24, 3, 1 };
    Vector<double> v3 { 2, 1.566 };

    v1 = v2;
    show(v1);

    v2 = std::move(v3);
    show(v2);
}

void compareOperatorTest()
{
    Vector<double> v1  { 2, 1.566, -123};
    Vector<double> v2 { 2, 1.566, -123 };
    Vector<double> v3 { 2, 1.566 };

    if ((v1 != v2) || (v1 == v3)) 
        std::cout << "compareOperatorTest has aborted" << std::endl;
}

void indexOperatorTest()
{
    Vector<int> v1 { 1, 2, 3, -4 };

    v1[0] = -123;

    if ((v1[3] != -4) || (v1[0] != -123)) 
        std::cout << "indexOperatorTest has aborted" << std::endl;
}

void resizeTest()
{
    Vector<int> v1 { 1, 2, 3, -4, 3 };

    v1.resize(2);
    size_t rs1 = v1.size();
    size_t cp1 = v1.capacity();
    show(v1);

    v1.resize(4);
    size_t rs2 = v1.size();
    size_t cp2 = v1.capacity();
    show(v1);

    v1.resize(10);
    size_t rs3 = v1.size();
    size_t cp3 = v1.capacity();
    show(v1);

    if (!((rs1 == 2) && (cp1 == 5)) || !((rs2 == 4) && (cp1 == 5)) || !((rs3 == 10) && (cp3 == 10))) 
        std::cout << "resizeTest has aborted" << std::endl;
}

void pushBackTest()
{
    Vector<int> v1 { 1, 2, 3, -4 };
    v1.push_back(1);

    int a = -123;
    v1.push_back(a);

    Vector<int> v2 { 1, 2, 3, -4, 1, -123 };

    if (v1 != v2)
        std::cout << "pushBackTest has aborted" << std::endl;
}

void popBackTest()
{
    Vector<double> v1 { 0, 1.2, 3, 5.3, 1.45 };

    double back = v1.pop_back();

    if ((back != 1.45))
        std::cout << "popBackTest has aborted" << std::endl;

}

void emplaceBackTest()
{
    struct Data
    {
        int size_;
        std::string name_;


        Data(): size_(0), name_("") {}
        Data(int size, const std::string& name): size_(size), name_(name) {}
    };

    Vector<Data> dat;
    dat.emplace_back(10, "MyDatabase");



    if ((dat.size() != 1) || (dat[0].size_ != 10) || (dat[0].name_ != "MyDatabase"))
        std::cout << "testEmplaceBack has aborted" << std::endl;
}

void isEmptyTest()
{
    Vector<int> v1;
    Vector<int> v2 { 1, 2, 3, 4 };
    Vector<int> v3 { 5, 6, 7 };
    v3.resize(0);

    if (!v1.empty() || v2.empty() || !v3.empty())
        std::cout << "isEmptyTest has aborted" << std::endl;
}

void sizeAndCapacityTest()
{
    Vector<int> v1 { 1, 2, 3 };
    v1.push_back(4);

    if ((v1.size() != 4) || (v1.capacity() != 6))
        std::cout << "sizeAndCapacityTest has aborted" << std::endl;
}

void clearTest()
{
    Vector<int> v1 {1, 2, 3};
    v1.clear();

    if (!v1.empty())
        std::cout << "clearTest has aborted" << std::endl;
}

int main()
{
    //tests

    /*constrTest(); //ok
    equalOperatorsTest(); // working but not use move-constr after std::move, wtf
    iteratorTest(); //ok
    compareOperatorTest(); //ok
    indexOperatorTest(); // ok
    resizeTest(); //ok
    pushBackTest(); //ok
    popBackTest(); //ok
    emplaceBackTest(); //ok 
    isEmptyTest(); //ok
    sizeAndCapacityTest(); //ok 
    clearTest(); //ok*/

    class Data
    {
        int size_;
        std::string name_;
        int* memory_;

    public:
        Data(): size_(0), name_(""), memory_(nullptr) {  std::cout << "data constr def" << std::endl;  }
        Data(int size, const std::string& name): size_(size), name_(name), memory_(new int[size])
        {
            std::cout << "data constr 2 par" << std::endl; 
        }

        Data(const Data& other): size_(other.size_), name_(other.name_)
        {   
            std::cout << "data copy constr" << std::endl; 

            memory_ = new int[other.size_];
            std::copy(other.memory_, other.memory_ + other.size_, memory_);
        }

        ~Data() 
        { 
            delete[] memory_;
            std::cout << "data destr" << std::endl; 
        }

        int size() { return size_; }
        const std::string& name() { return name_; }
    };


    Data x(1, "123"), y(2, "2"), z(3, "45");
    
    //Vector<Data> v { x, y, z };
    Vector<Data> v1(3, x);
    //Vector<Data> v2 { y, z };
    
    Vector<Data> v3(v1);
    //Vector<int> v { 1, 2, 3 };
    /*
    
    Allocator<Data> a;
    Data* dat = a.allocate(4* sizeof(Data));
    //Data* second = a.allocate(2 * sizeof(Data));

    a.construct(dat, x);

    std::cout << "size = " << dat[0].size() << ", name = " << dat[0].name() << std::endl;
    //std::cout << "size = " << second[0].size() << ", name = " << second[0].name() << std::endl;

    a.destroy(dat);

    a.deallocate(dat, 4);*/
    /*
    std::cout << "CONSTRUCT VECTOR" << std::endl;
    Vector<Data> v;
    std::cout << "push 1" << std::endl;
    v.push_back(x);
    std::cout << "push 2" << std::endl;
    v.push_back(y);
    std::cout << "push 3" << std::endl;
    v.push_back(z);
    std::cout << "EXIT" << std::endl;

    /*
    //auto v1 = std::vector<int> {1, 2, 3, 4, 5};
    auto vec = Vector<int> {1, 2, 3, 4, 5};

    //auto vec = Vector<int>(new int[5] {1, 2, 3, 4, 5}, 5) NOW
    std::vector<int> v1;
    std::cout << v1.size() << ' ' << v1.capacity() << std::endl;

    std::cout << v1.size() << ' ' << v1.capacity() << std::endl;

    /*v1.resize(4);
    std::cout << v1.size() << ' ' << v1.capacity() << std::endl;

    v1.resize(10);
    std::cout << v1.size() << ' ' << v1.capacity() << std::endl;
    

    for (auto i = v1.begin(); i != v1.end(); ++i)
    {
        std::cout << *i << " ";
    }

    /*std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;

    std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;*/

    

    /*auto v2 = std::vector<int> {1, 2, 3, 1, 2,2};
    v2.push_back(4);

    std::cout << "size = " << v2.size() << "\ncapasity = " << v2.capacity() << std::endl;

    v1 = v2;

    std::cout << "size = " << v1.size() << "\ncapasity = " << v1.capacity() << std::endl;*/

    return 0;
}