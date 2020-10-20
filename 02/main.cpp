#include <iostream>
#include "parser.hpp"

void testCustomCallbackFunctions()
{
    TokenParser tk;

    size_t start = 0, finish = 0, digit = 0, string = 0;

    tk.setStartCallback([&](){ ++start; });
    tk.setFinishCallback([&](){ ++finish; });
    tk.setDigitTokenCallback([&](u_int64_t){ ++digit; });
    tk.setStringTokenCallback([&](std::string){ ++string; });

    tk.parse("-90\n123 nfssdf34\t4561 567 stream0 3456 23 4\t67aa\n34afgh\n234a a2245");

    if (!((start == 1) && (finish == 1) && (digit == 6) && (string == 7)))
    {
        std::cout << "testCustomCallbackFunctions has aborted\n";
    } 
}

void testWorkingWithTokens()
{
    TokenParser tk;

    std::string string = "";
    u_int64_t digitSum = 0;

    tk.setStartCallback([&](){});
    tk.setFinishCallback([&](){});
    tk.setDigitTokenCallback([&](u_int64_t token){ digitSum += token; });
    tk.setStringTokenCallback([&](std::string token){ string += token; });
    
    tk.parse("1a 10\nbba2\t35 46\n578ta ke354 -231\t=123 60\n56 tra\nnse\tr3");

    if (!((digitSum == 207) && (string == "1abba2578take354-231=123transer3")))
    {
        std::cout << "testWorkingWithTokens has aborted\n";
    }
}

void testOnlySpaceSymbols()
{
    TokenParser tk;

    size_t start = 0, finish = 0, digit = 0, string = 0;

    tk.setStartCallback([&](){ ++start; });
    tk.setFinishCallback([&](){ ++finish; });
    tk.setDigitTokenCallback([&](u_int64_t){ ++digit; });
    tk.setStringTokenCallback([&](std::string){ string; });

    tk.parse("\n   \t \t  \n \t\t   \t\n\n\n \n");

    if (!((start == 1) && (finish == 1) &&  (digit == 0) &&  (string == 0))) 
    {
        std::cout << "testOnlySpaceSymbols has aborted\n";
    }
}

int main()
{
    testCustomCallbackFunctions();
    testWorkingWithTokens();
    testOnlySpaceSymbols();

    std::cout << "Comlete!" << std::endl;
    return 0;
}
