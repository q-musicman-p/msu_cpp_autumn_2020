#pragma once

#include <ostream>
#include "vector.hpp"

class Matrix : Vector
{
public:

    Matrix(u_int16_t _rows, u_int16_t _columns): rows(_rows), columns(_columns) {};

    u_int16_t getRows() const { return rows; }
    u_int16_t getColumns() const { return columns; }

    Matrix& operator+(Matrix& mat);

    Matrix& operator*=(int64_t num);

    bool operator==(Matrix& mat);

    bool operator!=(Matrix& mat) { return !operator==(mat); }

    int64_t& operator[](const size_t index);

    std::ostream show(Matrix& mat);
    
    ~Matrix()
    {
        Vector v;
        // ???
        for (size_t i = 0; i < rows; i++)
        {
            delete[] matrix[i];
        }
        
        delete[] matrix;
    }

private:
    int64_t** matrix;

    u_int16_t rows;
    u_int16_t columns;
};