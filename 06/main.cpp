#include <iostream>
#include "format.hpp"

void testWorking1()
{
    auto text = format("{1}+{1} = {0}", 2, "one");
    if (text != "one+one = 2") std::cout << "testWorking1 has aborted" << std::endl;
}

void testWorking2()
{
    auto text = format("{0} + {1} + {2} - {3} = {0}, but {6} - {5} = {1}", '0', 1, 2, 3, '4', 5, "6");
    if (text != "0 + 1 + 2 - 3 = 0, but 6 - 5 = 1") std::cout << "testWorking2 has aborted" << std::endl;
}

void testWasteParametrs()
{
    auto text = format("{0} + {2} = {0}", 0, 3, -5, 1, 2, 3, 4, 4);
    if (text != "0 + -5 = 0") std::cout << "testWasteParametrs has aborted" << std::endl;
}

void testFormatError1()
{
    try
    {
        auto text = format("{ error {1}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::UncorrectFormatingError) std::cout << "testFormatError2 has aborted" << std::endl;
    }
    catch(...)
    {
        std::cout << "testFormatError1 has aborted" << std::endl;
    }
}

void testFormatError2()
{
    try
    {
        auto text = format("error {1}}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::UncorrectFormatingError) std::cout << "testFormatError2 has aborted" << std::endl;
    }
}

void testUnpositiveIndexError()
{
    try
    {
        auto text = format(" error? {-423}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::UncorrectNumberInBracketsError) std::cout << "testUnpositiveIndexError has aborted" << std::endl;
    }
}

void testInvalidNumberError()
{
    try
    {
        auto text = format(" error? {asd1232}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::UncorrectNumberInBracketsError) std::cout << "testInvalidNumberError has aborted" << std::endl;
    }
}

void testEmptyBrackets()
{
    try
    {
        auto text = format(" error?{1} {} {0}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::UncorrectNumberInBracketsError) std::cout << "testEmptyBrackets has aborted" << std::endl;
    }
}

void testLessParametrs()
{
    try
    {
        auto text = format(" erro{0}r? {3}", 1, 2);
    }
    catch(const FormatException& e) 
    {
        if (e.getError() != FormatException::Error::ParametrsCountError) std::cout << "testLessParametrs has aborted" << std::endl;
    }
}

int main()
{
    testWorking1();
    testWorking2();
    testWasteParametrs();
    testFormatError1();
    testFormatError2();
    testUnpositiveIndexError();
    testInvalidNumberError();
    testEmptyBrackets();
    testLessParametrs();

    std::cout << "Complete!" << std::endl;

    return 0;
}