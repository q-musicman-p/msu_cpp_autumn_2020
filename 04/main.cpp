#include "bignumber.hpp"
#include <iostream>
#include <sstream>


void testCompare()
{
    BigNumber a("1234567890000001");
    BigNumber b("1234567890000001");
    BigNumber c("1234567890000002");
    BigNumber d("1234");
    BigNumber e("-124564");
    BigNumber f("-134567");
    BigNumber g("134567");

    if (!(a == b) || !(b < c) || !(d < a) || !(e < d) || !(f < e) || (f == g)) std::cout << "testCompareSimilarSign has aborted!\n";
}

void testOperatorPrefixMinus()
{
    BigNumber a = -12345;

    if (-a != 12345) std::cout << "testOperatorPrefixMinus has aborted!\n";
}

void testOperatorPlusSimilarSigns()
{
    BigNumber a("12345678");
    BigNumber b("93456789");
    BigNumber c("105802467");

    if (((a + b) != c) || ((-a + -b) != -c)) std::cout << "testOperatorPlusSimilarSigns has aborted!\n";
}

void testOperatorMinusSimilarSigns()
{
    BigNumber a("1200067458");
    BigNumber b("3456786791");
    BigNumber c("-2256719333");

    if (((a - b) != c) || (-a - (-b) != -c)) std::cout << "testOperatorMinusSimilarSigns has aborted!\n";
}

void testOperatorPlusDiffSigns()
{
    BigNumber a("123456780");
    BigNumber b("-934567890");
    BigNumber c("-811111110");

    if (((a + b) != c) || (-a + (-b) != -c)) std::cout << "testOperatorPlusDiffSigns has aborted!\n";
}

void testOperatorMinunsDiffSigns()
{
    BigNumber a("123456780");
    BigNumber b("-934567890");
    BigNumber c("1058024670");

    if (((a - b) != c) || (-a - (-b) != -c)) std::cout << "testOperatorMinunsDiffSigns has aborted!\n";
}

void testCopyOperatorEqual()
{
    BigNumber a("1234567890123");
    BigNumber b("123456");

    a = b;

    if (a != b) std::cout << "testCopyOperatorEqual has aborted!\n";
}

void testCopyConstr()
{
    BigNumber a("1234567890123");
    BigNumber b = a;

    a = a - 123;

    if (a == b) std::cout << "testCopyConstr has aborted!\n";
}

void testMoveConst()
{
    BigNumber a("1234567890123");
    BigNumber b = std::move(a);

    if (b != 1234567890123) std::cout << "testMoveConst has aborted!\n";
}

void testMoveOperatorEqual()
{
    BigNumber a("1234567890123");
    BigNumber b;
    b = std::move(a);

    if (b != 1234567890123) std::cout << "testMoveOperatorEqual has aborted!\n";
}

void testOperatorMulty()
{
    BigNumber a("1234567890");
    BigNumber b = 432;
    BigNumber c("533333328480");

    if ((a * b != c) || (a * (-b) != -c) || (-a * b != -c) || (-a * (-b) != c)) std::cout << "testOperatorMulty has aborted!\n";
}

void testOperatorStream()
{
    std::stringstream ss;
    BigNumber a("1234567890");
    BigNumber b("1230000000");
    ss << (b - a) << " " << b;

    if (ss.str() != "-4567890 1230000000") std::cout << "testOperatorStream has aborted!\n";
}

int main()
{
    // Tests
    testCompare();
    testOperatorPrefixMinus();
    testOperatorPlusSimilarSigns();
    testOperatorMinusSimilarSigns();
    testOperatorPlusDiffSigns();
    testOperatorMinunsDiffSigns();
    testCopyOperatorEqual();
    testCopyConstr();
    testMoveConst();
    testMoveOperatorEqual();
    testOperatorMulty();
    testOperatorStream();

    std::cout << "Complete!" << std::endl;

    return 0;
}