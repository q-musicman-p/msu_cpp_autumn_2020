#pragma once

#include <iostream>

template <class T>
class Vector
{
private:
    size_t size_;
    size_t capacity_;
    T* memory_;

    class Iterator;
    class Allocator;
public:
    explicit Vector(size_t size);
    Vector(size_t size, const T& default_value);
    Vector(T* array, size_t size);
    
    Vector(const Vector& other);
    Vector(Vector&& other);

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector operator=(Vector&& other);

    bool operator==(const Vector& other) const;

    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    void push_back(const T& el);
    void push_back(T&& el);

    const T& pop_back();

    template <class... Args>
    void emplace_back(Args&&... args); //universal referense

    bool empty() const noexcept;

    size_t size() const noexcept; 
    size_t capacity() const noexcept;

    void clear() noexcept;

    Iterator& begin() noexcept;
    const Iterator& begin() const noexcept;

    Iterator& end() noexcept;
    const Iterator& end() const noexcept;

    Iterator& rbegin() noexcept;
    const Iterator& rbegin() const noexcept;

    Iterator& rend() noexcept;
    const Iterator& rend() const noexcept;

    void resize(const size_t size);

    void reserve(const size_t capacity);
private:
    template <class... Args>
    void emplace_split(T&& object, Args&&... args);
};

// ======================================
// ============ CONSTRUCTORS ============
// ======================================

template <class T>
Vector<T>::Vector(size_t size): size_(size), capacity_(size), memory_(new T[size]) { std::cout << "C_size" << std::endl; }

template <class T>
Vector<T>::Vector(size_t size, const T& default_value): size_(size), capacity_(size), memory_(new T[size])
{
    std::cout << "C_size_def" << std::endl;

    //std::fill(begin(), end(), T());
    std::fill(memory_, memory_ + size_, default_value);
    //for (size_t i = 0; i < size; i++)
    //{
    //    memory_[i] = T();
    //}
}

template <class T>
Vector<T>::Vector(T* array, size_t size): size_(size), capacity_(size), memory_(array) {std::cout << "C_arr" << std::endl;}

template <class T>
Vector<T>::Vector(const Vector& other): size_(other.size_), capacity_(other.capacity_), memory_(new T[other.capacity_])
{
    std::cout << "C_copy" << std::endl;

    //std::copy(other.begin(), other.end(), memory_);
    std::copy(other.memory_, other.memory_ + other.capacity_, memory_);
}

template <class T>
Vector<T>::Vector(Vector&& other): size_(other.size_), capacity_(other.capacity_)
{
    std::cout << "C_rvalue" << std::endl;

    memory_ = new(other.memory_) T[other.capacity_];

    other.size_ = 0;
    other.capacity_ = 0;
    other.memory_ = nullptr;
}

template <class T>
Vector<T>::~Vector()
{
    std::cout << "Destr" << std::endl;

    delete[] memory_;
}

// ======================================
// ============= OPERATORS ==============
// ======================================

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    std::cout << "copy_operator==" << std::endl;

    // self assign
    if (*this == other)
        return *this;

    // we can no allocate new memory if it not needed
    if (other.size_ > capacity_)
    {
        T* temp = new T[other.size_];
        delete[] memory_;
        memory_ = temp;
        
        capacity_ = other.size_;
    }

    std::copy(other.memory_, other.memory_ + other.size_, memory_);
    size_ = other.size_;

    return *this;
}

template <class T>
Vector<T> Vector<T>::operator=(Vector&& other)
{
    std::cout << "rvalue_operator=" << std::endl;

    // self assign
    if (*this == other)
        return *this;

    delete[] memory_;
    memory_ = other.memory_;
    size_ = std::move(other.size_);
    capacity_ = std::move(other.capacity_);

    other.memory_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

template <class T>
bool Vector<T>::operator==(const Vector& other) const
{
    std::cout << "operator==" << std::endl;

    if (other.size_ != size_)
        return false;
    
    for (size_t i = 0; i < size_; i++)
    {
        if (memory_[i] != other.memory_[i])
            return false;
    }
    
    return true;
}

template <class T>
T& Vector<T>::operator[](const size_t index)
{
    return memory_[index];
}

template <class T>
const T& Vector<T>::operator[](const size_t index) const
{
    return memory_[index];
}

// ======================================
// =============== METHODS ==============
// ======================================

template <class T>
size_t Vector<T>::size() const noexcept
{
    return size_;
}

template <class T>
size_t Vector<T>::capacity() const noexcept
{
    return capacity_;
}

template <class T>
bool Vector<T>::empty() const noexcept
{
    return size_ == 0;
}

template <class T>
void Vector<T>::clear() noexcept
{
    std::fill(memory_, memory_ + size_, T());
}

template <class T>
void Vector<T>::resize(const size_t size)
{
    if (size > size_)
    {
        capacity_ *= 2;
        T* temp = new T[capacity_];
        std::copy(memory_, memory_ + size_, temp);

        delete[] memory_;
        memory_ = temp;
    }

    size_ = size;
}

template <class T>
void Vector<T>::reserve(const size_t capacity)
{
    if (capacity > capacity_)
    {
        T* temp = new T[capacity];
        std::copy(memory_, memory_ + size_, temp);

        delete[] memory_;
        memory_ = temp;

        capacity_ = capacity;
    }
}

template <class T>
const T& Vector<T>::pop_back()
{
    return memory_[size_--];
}

template <class T>
void Vector<T>::push_back(const T& el)
{
    if (capacity_ == size_)
        reserve(2 * capacity_);
        
    memory_[size_ + 1] = el;
    size_++;
}

template <class T>
void Vector<T>::push_back(T&& el)
{
    if (capacity_ == size_)
        reserve(2 * capacity_);
        
    memory_[size_ + 1] = std::move(el);
    size_++;
}
    
template <class T>
template <class... Args>
void Vector<T>::emplace_back(Args&&... args)
{
    emplace_split(args...);
}

template <class T>
template <class... Args>
void Vector<T>::emplace_split(T&& object, Args&&... args)
{
    if (capacity_ == size_)
        reserve(2 * capacity_);
        
    memory_[size_ + 1] = std::forward<T>(T(args...));
    size_++;
}