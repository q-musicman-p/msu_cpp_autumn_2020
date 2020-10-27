#include <iostream>
#include <sstream>
#include "matrix.hpp"

void testShow()
{
    u_int16_t m = 2, n = 3;

    int64_t** arr = new int64_t*[m];
    arr[0] = new int64_t[n] {1, 2, 3};
    arr[1] = new int64_t[n] {4, 5, 6};

    Matrix mat(m, n, arr);
    std::stringstream stream;
    stream << mat;

    if (stream.str() != "\n1 2 3 \n4 5 6 ") std::cout << "testShow has aborted\n";
}

void testCreatigEmpryMatrix()
{
    Matrix zero_mat(3, 5);
    std::stringstream stream;
    stream << zero_mat;

    if (stream.str() != "\n0 0 0 0 0 \n0 0 0 0 0 \n0 0 0 0 0 ") std::cout << "testCreatigEmpryMatrix has aborted\n";
}

void testIsEqual()
{
    /*int64_t** arr = new int64_t*[2];
    arr[0] = new int64_t[3] {1, -3, 5};
    arr[1] = new int64_t[3] {0, 1, -4};

    Matrix m1(2, 3, arr), m2(2, 3, arr);*/
    Matrix m1(4, 5), m2(4, 5);

    if ((m1 == m2) == false) std::cout << "testIsEqual has aborted\n";
}

void testOperatorPlus()
{
    int64_t** arr1 = new int64_t*[2];
    arr1[0] = new int64_t[3] {1, 2, 3};
    arr1[1] = new int64_t[3] {4, 5, 6};

    int64_t** arr2 = new int64_t*[2];
    arr2[0] = new int64_t[3] {1, -3, 5};
    arr2[1] = new int64_t[3] {0, 1, -4};
    
    Matrix A(2, 3, arr1);
    Matrix B(2, 3, arr2);

    int64_t** sum_arr = new int64_t*[2];
    sum_arr[0] = new int64_t[3] {2, -1, 8};
    sum_arr[1] = new int64_t[3] {4, 6, 2};

    Matrix sum(2, 3, sum_arr);

    if ((A + B) != sum) std::cout << "testOperatorPlus has aborted\n";
}

void testOperatorMulOnNum()
{
    int64_t** arr = new int64_t*[2];
    arr[0] = new int64_t[3] {1, -3, 5};
    arr[1] = new int64_t[3] {0, 1, -4};

    Matrix A(2, 3, arr);
    A *= -5;
    
    int64_t** arr_m = new int64_t*[2];
    arr_m[0] = new int64_t[3] {-5, 15, -25};
    arr_m[1] = new int64_t[3] {0, -5, 20};

    Matrix A_mul_minus_five(2, 3, arr_m);

    if (A != A_mul_minus_five) std::cout << "testOperatorMulOnNum has aborted\n";
}

void testGets()
{
    Matrix mat(3, 4);

    if ((mat.getRows() != 3) || (mat.getColumns() != 4)) std::cout << "testGets has aborted\n";
}

void testGetAndChangeElement()
{
    int64_t** arr = new int64_t*[2];
    arr[0] = new int64_t[3] {1, -3, 5};
    arr[1] = new int64_t[3] {3, 1, -4};

    Matrix A(2, 3, arr);

    A[0][2] = 999;

    if ((A[1][0] != 3) || (A[0][2] != 999)) std::cout << "testGetAndChangeElement has aboreted\n";
}

void testTryToSumDiffShape()
{
    Matrix m1(1, 2);
    Matrix m2(2, 1);

    std::string error;

    try
    {
        Matrix m3 = m1 + m2;
    }
    catch(const std::exception& e)
    {
        error = e.what();
    }

    if (error != "Can't sum matrixes with different shapes!\n") std::cout << "testTryToSumDiffShape has aborted\n";
}

void testIndexOutOfRange()
{
    Matrix mat(3, 4);
    int error_count = 0;

    try
    {
        int64_t t = mat[0][4];
    }
    catch(const std::exception& e)
    {
        
        error_count++;
    }

    try
    {
        int64_t t = mat[3][1];
    }
    catch(const std::exception& e)
    {
        error_count++;
    }

    if (error_count < 2) std::cout << "testIndexOutOfRange has aborted\n";
}

int main()
{
    // Tests

    testShow();
    testCreatigEmpryMatrix();
    testGets();
    testIsEqual();
    testOperatorPlus();
    testOperatorMulOnNum();
    testGetAndChangeElement();
    testTryToSumDiffShape();
    testIndexOutOfRange();

    std::cout << "Complete!" << std::endl;

    return 0;
}