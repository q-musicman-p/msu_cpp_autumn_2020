#include <iostream>
#include "thread_pool.hpp"
#include <unistd.h>

struct A {};

void foo(const A&) {};

int matrix_sum(int n, int coef)
{
    double** matrix = new double*[n];
    for (size_t i = 0; i < n; i++)
        matrix[i] = new double[n];

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            matrix[i][j] = coef * (i + j);
        }
    }
    
    int sum = 0;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            sum += matrix[i][j];
        }
    }

    for (size_t i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;

    return sum;
}

void error_function()
{
    throw std::out_of_range("SORRY");
}

int main()
{
    ThreadPool pool(4);

    auto task1 = pool.exec(foo, A());
    task1.get();
    
    auto task2 = pool.exec([]() { return 1; });
    if (task2.get() != 1)
        std::cout << "Error in lambda function!" << std::endl;
    
    int size = 10;
    std::vector<std::future<int>> task;
    for (size_t i = 0; i < size; i++)
    {
        task.emplace_back(pool.exec(matrix_sum, 100*(i + 1), i));
    }
    
    auto error_task = pool.exec(error_function);

    bool error = false;
    try
    {
        error_task.get();
    }
    catch(const std::exception& e)
    {
        error = true;
    }

    if (!error)
        std::cout << "Error in error_task!" << std::endl;
    
    int sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        sum += task[i].get();
    }
    
    if (sum != 806751622)
        std::cout << "Error in matrix sum!" << std::endl;


    std::cout << "Comlete!" << std::endl;
    return 0;
}