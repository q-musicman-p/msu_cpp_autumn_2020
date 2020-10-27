#include <iostream>
#include "matrix.hpp"

int main()
{
    int64_t** arr1 = new int64_t*[2];
    arr1[0] = new int64_t[3] {1, 2, 3};
    arr1[1] = new int64_t[3] {4, 5, 6};

    int64_t** arr2 = new int64_t*[2];
    arr2[0] = new int64_t[3] {1, -3, 5};
    arr2[1] = new int64_t[3] {0, 1, -4};
    
    Matrix A(2, 3, arr1);
    Matrix B(2, 3, arr2);

    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n";

    Matrix C = A + B;

    std::cout << "C = A + B = " << C << "\n";

    std::cout << "A[1][2] = " << A[1][2] << "\n";
    std::cout << "A[1][2] the = 784\n";
    A[1][2] = 784;
    std::cout << "A[1][2] = " << A[1][2] << "\n";
    
    // Three is will be tests =)

    /*Vector* vect1 = new Vector[3] {Vector(2), Vector(2), Vector(2)};
    Vector* vect2 = new Vector[3] {Vector(2), Vector(2), Vector(2)}; 

    Matrix m1(vect1), m2(vect2);

    //Matrix m3 = m1 + m2;

    delete[] vect1;
    delete[] vect2;

    /*Vector v1(3, new int64_t[3] {1, 2, 3}), v2(3, new int64_t[3] {3, -4, 5});
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
    std::cout << "Get v2[0]: " << v2[0];*/

    return 0;
}