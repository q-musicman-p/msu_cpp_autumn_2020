#include <iostream>
#include "vector.hpp"

void iteratorTest()
{
    Vector<double> v { 1.2, 3, 5.4, 1 };

    double* vec = new double[4] { 1.2, 3, 5.4, 1 };
    double* reverse_vec = new double[4] { 1, 5.4, 3, 1.2 };

    bool error = false;
    int j = 0;

    for (auto i = v.begin(); i != v.end(); ++i)
    {
        if (vec[j] != *i)
        {
            error = true;
            break;
        }

        j++;
    }

    bool reverse_error = false;
    j = 0;

    for (auto i = v.rbegin(); i != v.rend(); ++i)
    {
        if (reverse_vec[j] != *i)
        {
            error = true;
            break;
        }

        j++;
    }

    delete[] vec;
    delete[] reverse_vec;

    if (error || reverse_error)
        std::cout << "iteratorTest has aborted" << std::endl;
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

/////////////////////////////////////////////////////////////////////////////////////////

static int count;
static void counter(int code)
{
    switch (code)
    {
    case 0:
        count = 0;
        break;
    case 1:
        count++;
        break;
    case -1:
        count--;
        break;
    default:
        throw "count error";
        break;
    }
}

class Data
{
    int size_;
    std::string name_;
    int* memory_;

public:
    Data(): size_(0), name_(""), memory_(nullptr) 
    {
        counter(1);
    }
    Data(int size, const std::string& name): size_(size), name_(name), memory_(new int[size])
    {
        counter(1);
    }

    Data(const Data& other): size_(other.size_), name_(other.name_)
    {   
        counter(1);

        memory_ = new int[other.size_];
        std::copy(other.memory_, other.memory_ + other.size_, memory_);
    }

    Data(Data&& other): size_(std::move(other.size_)), name_(std::move(other.name_)), memory_(other.memory_)
    {
        counter(1);
        
        other.size_ = 0;
        other.name_ = "";
        other.memory_ = nullptr;
    }

    ~Data() 
    { 
        counter(-1);
        delete[] memory_;
    }

    int size() { return size_; }
    const std::string& name() { return name_; }
};

void inicializerConstrHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);
        Data x(1, "one"), y(2, "two"), z(3, "there");
        
        Vector<Data> v { x, y, z };

        if ((v[0].name() != "one") || (v[0].size() != 1) ||
            (v[1].name() != "two") || (v[1].size() != 2) ||
            (v[2].name() != "there") || (v[2].size() != 3))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "inicializerConstrHardTest has aborted" << std::endl;
}

void sizeConstrHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);
        Vector<Data> v(3);

        if ((v[0].name() != "") || (v[0].size() != 0) ||
            (v[1].name() != "") || (v[1].size() != 0) ||
            (v[2].name() != "") || (v[2].size() != 0))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "sizeConstrHardTest has aborted" << std::endl;
}

void defValueConstrHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data def(43, "new");
        Vector<Data> v(3, def);

        if ((v[0].name() != "new") || (v[0].size() != 43) ||
            (v[1].name() != "new") || (v[1].size() != 43) ||
            (v[2].name() != "new") || (v[2].size() != 43))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "defValueConstrHardTest has aborted" << std::endl;
}

void copyConstrHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two");
        Vector<Data> v { x, y };
        Vector<Data> v_copy = v;

        if ((v[0].name() != v_copy[0].name()) || (v[0].size() != v_copy[0].size()) ||
            (v[1].name() != v_copy[1].name()) || (v[1].size() != v_copy[1].size()))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "copyConstrHardTest has aborted" << std::endl;
}

void moveConstrHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two");
        Vector<Data> v { x, y };
        Vector<Data> v_move = std::move(v);

        if (("one" != v_move[0].name()) || (1 != v_move[0].size()) ||
            ("two" != v_move[1].name()) || (2 != v_move[1].size()) ||
            (!v.empty()))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "moveConstrHardTest has aborted" << std::endl;
}

void copyEqualOperatorHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two");
        Vector<Data> v { x, y };
        Vector<Data> v_copy(3);
        v_copy = v;

        if ((v[0].name() != v_copy[0].name()) || (v[0].size() != v_copy[0].size()) ||
            (v[1].name() != v_copy[1].name()) || (v[1].size() != v_copy[1].size()))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "moveConstrHardTest has aborted" << std::endl;
}

void moveEqualOperatorHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x, y };
        Vector<Data> v_move { z };
        v_move = std::move(v);

        if (("one" != v_move[0].name()) || (1 != v_move[0].size()) ||
            ("two" != v_move[1].name()) || (2 != v_move[1].size()) ||
            (!v.empty()))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "moveConstrHardTest has aborted" << std::endl;
}

void clearHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x, y, z };
        v.clear();

        if (!v.empty())
            error = true;
    }

    if ((count != 0) || error)
        std::cout << "clearHardTest has aborted" << std::endl;
}

void resizeHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x, y, z };

        v.resize(1);
        size_t rs1 = v.size();
        size_t cp1 = v.capacity();

        v.resize(2);
        size_t rs2 = v.size();
        size_t cp2 = v.capacity();

        v.resize(4);
        size_t rs3 = v.size();
        size_t cp3 = v.capacity();

        if (!((rs1 == 1) && (cp1 == 3)) || !((rs2 == 2) && (cp2 == 3)) || !((rs3 == 4) && (cp3 == 4)) ||
            (v[1].name() == "two") || (v[1].size() == 2) ||
            (v[3].name() != "") || (v[3].size() != 0))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "resizeHardTest has aborted" << std::endl;
}

void reserveHardTest()
{
    bool error = false;
    // Testing zone
    {   
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x, y, z };

        v.reserve(2);
        size_t rs1 = v.size();
        size_t cp1 = v.capacity();

        v.reserve(6);
        size_t rs2 = v.size();
        size_t cp2 = v.capacity();
       
        if (!((rs1 == 3) && (cp1 == 3)) || !((rs2 == 3) && (cp2 == 6)))
                error = true;
    }
    
    if ((count != 0) || error)
        std::cout << "reserveHardTest has aborted" << std::endl;
}

void pushBackHardTest()
{
    bool error = false;
    {
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x };
        v.push_back(y);

        v.push_back(std::move(z));

        if ((v[0].name() != "one") || (v[0].size() != 1) ||
            (v[1].name() != "two") || (v[1].size() != 2) ||
            (v[2].name() != "three") || (v[2].size() != 3) ||
            (v.size() != 3) || (v.capacity() != 4))
                error = true;
    }

    if ((count != 0) || error)
        std::cout << "pushBackHardTest has aborted" << std::endl;
}

void popBackHardTest()
{
    bool error = false;
    {
        counter(0);

        Data x(1, "one"), y(2, "two"), z(3, "three");
        Vector<Data> v { x, y, z };
        v.pop_back();

        if ((v.size() != 2) || (v.capacity() != 3))
                error = true;
    }

    

    if ((count != 0) || error)
        std::cout << "pushBackHardTest has aborted" << std::endl;

}

void emplaceBackHardTest()
{
    bool error = false;
    {
        counter(0);

        Data x(1, "one"), y(2, "two");
        Vector<Data> v { x, y };

        v.emplace_back(3, "three");

        if ((v[2].name() != "three") || (v[2].size() != 3) ||
            (v.size() != 3) || (v.capacity() != 4))
                error = true;
    }

    if ((count != 0) || error)
        std::cout << "emplaceBackTest has aborted" << std::endl;
}

int main()
{
    // SIMPLE TESTS
    iteratorTest(); 
    compareOperatorTest(); 
    indexOperatorTest(); 
    isEmptyTest(); 
    sizeAndCapacityTest(); 

    //HARD TESTS
    inicializerConstrHardTest();
    sizeConstrHardTest();
    defValueConstrHardTest();
    copyConstrHardTest();
    moveConstrHardTest();
    copyEqualOperatorHardTest();
    moveEqualOperatorHardTest();
    clearHardTest();
    resizeHardTest();
    reserveHardTest();
    pushBackHardTest();
    popBackHardTest();
    emplaceBackHardTest();//*/

    std::cout << "Complete!" << std::endl;

    return 0;
}