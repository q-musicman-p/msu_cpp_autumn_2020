#include <ostream>
#include <iostream>

class Vector
{
public:
    Vector(): size(1), data(new int64_t[1]) {};

    Vector(u_int16_t _size): size(_size), data(new int64_t[_size]) {};

    Vector(u_int16_t _size, int64_t* array):  size(_size), data(array) {}; // ???

    virtual ~Vector() 
    { 
        delete[] data; 
    }

    u_int16_t getSize() const { return size; }

    const Vector& operator=(const Vector& v) const
    {
        return Vector(v.size, v.data);
    }

    int64_t& operator[](const size_t index) { return data[index]; }

    const int64_t& operator[](const size_t index) const { return data[index]; }

    const Vector& operator+(const Vector& v) const
    {
        if (size != v.size) throw "Can't sum different size's vectors!";

        int64_t* new_data = new int64_t[size];
        for (size_t i = 0; i < size; i++) new_data[i] = data[i] + v.data[i];

        return Vector(size, new_data);
    }

    Vector& operator+=(const Vector& v)
    {
        if (size != v.size) throw "Can't sum different size's vectors!";

        for (size_t i = 0; i < size; i++) data[i] += v.data[i];

        return *this;
    }

    const Vector& operator*(const int64_t c) const
    {
        int64_t* new_data = new int64_t[size];
        for (size_t i = 0; i < size; i++) new_data[i] = data[i] * c;

        return Vector(size, new_data);
    }

    Vector& operator*=(const int64_t c)
    {
        for (size_t i = 0; i < size; i++) data[i] *= c;

        return *this;
    }
    
    bool operator==(const Vector& v) const
    {
        if (size != v.size) throw "Can't compare different size's vectors!";
        bool isEqual = true;
        
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] != v.data[i])
            {
                isEqual = false;
                break;
            }
        }
        
        return isEqual;
    }

    bool operator!=(const Vector& v) const { return !operator==(v); }

protected:
    int64_t* data;
    uint16_t size;
};

const Vector& operator*(int64_t c, const Vector& v) { return v*c; }

std::ostream& operator<<(std::ostream& stream, const Vector& v)
{
    for (size_t i = 0; i < v.getSize() - 1; i++)
    {
        stream << v[i] << " ";
    }
    stream << v[v.getSize() - 1];
    
    return stream;
}

