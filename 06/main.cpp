#include <iostream>
#include "format.hpp"

int main()
{
    auto text = format("{1}+{1} = {0}", 2, "one");
    std::cout << "text = " << text << std::endl;

    return 0;
}