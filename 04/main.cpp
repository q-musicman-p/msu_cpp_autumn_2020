#include "bignumber.hpp"
#include <iostream>

int main()
{   
    // Tests
    BigNumber b1 = 23451;
    BigNumber b2 = 12340;
    
    b1 = b2;

    std::cout << "b1 = " << b1 << " b2 = " << b2 << std::endl;

    std::cout << "BigNumber = " << (b1 + b2) << std::endl;

    std::cout << "Complete!" << std::endl;
    return 0;
}