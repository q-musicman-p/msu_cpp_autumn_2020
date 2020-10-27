#pragma once

#include <ostream>

class Matrix
{
public:
    Matrix(u_int16_t _rows, u_int16_t _columns): rows(_rows), columns(_columns)
    {
        data = new int64_t*[_rows];
        for (size_t i = 0; i < _rows; i++) data[i] = new int64_t[_columns];

        for (size_t i = 0; i < _rows; i++)
        {
            for (size_t j = 0; j < _columns; j++)
            {
                data[i][j] = 0;
            }
        }
    }

    Matrix(u_int16_t _rows, u_int16_t _columns, int64_t** _data): rows(_rows), columns(_columns), data(_data)
    {
        data = new int64_t*[_rows];
        for (size_t i = 0; i < _rows; i++) data[i] = new int64_t[_columns];

        for (size_t i = 0; i < _rows; i++)
        {
            for (size_t j = 0; j < _columns; j++)
            {
                data[i][j] = _data[i][j];
            }
        }
    }

    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++) { delete[] data[i]; }
        delete[] data;
    }

    u_int16_t getRows() const { return rows; }
    u_int16_t getColumns() const { return columns; }

    Matrix operator+(const Matrix& mat) const
    {
        if ((rows != mat.rows) || (columns != mat.columns)) 
        {
            throw std::out_of_range("Can't sum matrixes with different shapes!\n");
        }

        int64_t** new_data = new int64_t*[rows];
        for (size_t i = 0; i < rows; i++) new_data[i] = new int64_t[columns];

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                new_data[i][j] = data[i][j] + mat.data[i][j];
            }
        }

        return Matrix(rows, mat.columns, new_data);
    }

    Matrix& operator*=(int64_t num)
    {
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                data[i][j] *= num;
            }
        }

        return *this;
    }

    bool operator==(const Matrix& mat) const
    {
        if ((rows != mat.rows) || (columns != mat.columns)) 
        {
            throw std::out_of_range("Can't compare matrixes with different shapes!\n");
        }

        bool isEqual = true;

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                if (data[i][j] != mat.data[i][j])
                {
                    isEqual = false;
                    break;
                }
            }
            if (!isEqual) break;
        }

        return isEqual;
    }

    bool operator!=(Matrix& mat) const { return !operator==(mat); }

    private: class Proxy;
    public:
    Proxy operator[](const size_t index) const
    {
        if (rows <= index) throw std::out_of_range("");

        return Proxy(index, columns, data);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);

private:
    class Proxy
    {
    public:
        Proxy(size_t _row_index, u_int16_t _columns, int64_t** _data): 
            row_index(_row_index), columns(_columns), data(_data) {}
        
        int64_t& operator[](const size_t column_index)
        {
            if (columns <= column_index) throw std::out_of_range("");

            return data[row_index][column_index];
        }

        const int64_t& operator[](const size_t column_index) const
        {
            if (columns <= column_index) throw std::out_of_range("");

            return data[row_index][column_index];
        }
    private:
        int64_t** data;

        u_int16_t columns;
        size_t row_index;
    };

    int64_t** data;

    u_int16_t rows;
    u_int16_t columns;
};

std::ostream& operator<<(std::ostream& stream, const Matrix& m)
{
    for (size_t i = 0; i < m.rows; i++)
    {
        stream << '\n';
        for (size_t j = 0; j < m.columns; j++)
        {
            stream << m.data[i][j] << " ";
        }
    }

    return stream;      
}