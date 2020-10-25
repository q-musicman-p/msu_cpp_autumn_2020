#include <iostream>
#include "vector.hpp"

int main()
{
    // Three is will be tests =)


    Vector v1(3, new int64_t[3] {1, 2, 3}), v2(3, new int64_t[3] {3, -4, 5});
    std::cout << "v1 = " << v1 << "\nv2 = " << v2 << '\n';

    Vector v_sum = v1 + v2;
    std::cout << "v1 + v2 = " << v_sum << '\n';

    v_sum += v1;
    std::cout << "2v1 + v2 = " << v_sum << '\n';

    int c1 = -1;
    int c2 = 2;

    v1 *= 3;
    std::cout << "v1 *= 3: " << v1 << '\n';

    v1 = v1 * c1;
    std::cout << "v1 * (-1) = " << v1 << '\n';

    v2 = c2 * v2;
    std::cout << "2 * v2 = " << v2 << '\n';

    Vector v3(3, new int64_t[3] {1, 2, 3}), v4(3, new int64_t[3] {1, 2, 3});
    std::cout << "Test (1,2,3) == (1,2,3): " << (v3 == v4) << '\n';

    v1[2] = 54;
    std::cout << "Now v1[2] = 54, v1 =  " << v1 << '\n';
    std::cout << "Get v2[0]: " << v2[0];

    return 0;
}