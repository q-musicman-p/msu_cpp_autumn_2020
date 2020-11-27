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

    //dont fogot about self assigment!
    Vector& operator=(const Vector& other);
    Vector operator=(Vector&& other);

    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    void push_back(const T&);
    void push_back(T&&);

    const T& pop_back();

    template <class... Args>
    void emplace_back(Args&&... args); //universal referense

    bool empty() const noexcept { return size_ == 0; } 

    size_t size() const noexcept { return size_; } 
    size_t capacity() const noexcept { return capacity_; } 

    void clear() noexcept;

    Iterator& begin() noexcept;
    const Iterator& begin() const noexcept;

    Iterator& end() noexcept;
    const Iterator& end() const noexcept;

    Iterator& rbegin() noexcept;
    const Iterator& rbegin() const noexcept;

    Iterator& rend() noexcept;
    const Iterator& rend() const noexcept;

    void resize(size_t size);

    void reverse(size_t capacity);
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
Vector<T>::Vector(const Vector& other): size_(other.size_), capacity_(other.size_), memory_(new T[other.size_])
{
    std::cout << "C_copy" << std::endl;

    //std::copy(other.begin(), other.end(), memory_);
    std::copy(other.memory_, other.memory_ + other.size_, memory_);
}

template <class T>
Vector<T>::Vector(Vector&& other): size_(other.size_), capacity_(other.size_)
{
    std::cout << "C_rvalue" << std::endl;

    memory_ = new(other.memory_) T[other.size_];

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

}