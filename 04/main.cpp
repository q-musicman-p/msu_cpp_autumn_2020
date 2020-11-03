#include <iostream>
#include "bignumber.hpp"

int main()
{   
    // Tests
    BigNumber b1 = 178023;
    BigNumber b2 = 178000;
    // 632
    b1 = b2;

    std::cout << "b1 = " << b1 << " b2 = " << b2 << std::endl;

    std::cout << "BigNumber = " << (b1 - b2) << std::endl;

    std::cout << "Complete!" << std::endl;
    return 0;
}