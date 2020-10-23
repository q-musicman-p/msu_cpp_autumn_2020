#include <ostream>

class Matrix
{
public:
    Matrix(u_int16_t _rows, u_int16_t _columns): rows(_rows), columns(_columns) {};

    u_int16_t getRows() const { return rows; }
    u_int16_t getColumns() const { return columns; }

    Matrix& operator+(Matrix& mat);

    Matrix& operator*=(double num);

    bool operator==(Matrix& mat);

    bool operator!=(Matrix& mat) { return !operator==(mat); }

    double& operator[](Matrix& mat);

    std::ostream show(Matrix& mat);
    
    ~Matrix()
    {
        // ???
        for (size_t i = 0; i < rows; i++)
        {
            delete[] matrix[i];
        }
        
        delete[] matrix;
    }

private:
    double** matrix;

    u_int16_t rows;
    u_int16_t columns;
};