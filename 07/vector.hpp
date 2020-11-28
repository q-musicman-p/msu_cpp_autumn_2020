#pragma once

#include <iostream>
#include <initializer_list>

template <class T>
class Vector
{
    size_t size_;
    size_t capacity_;
    T* memory_;

    class Iterator
    {
        T* current_;
        bool reversed_;
    public:
        explicit Iterator(T* el, bool reversed = false): current_(el), reversed_(reversed) {}

        bool operator==(const Iterator& other) const 
        {
            if (this == &other)
            {
                return true;
            }
            
            return  current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const
        {
            return !operator==(other);
        }

        T& operator*() const
        {
            if (current_)
                return *current_;
            
            throw std::out_of_range("");
        }

        void operator++()
        {
            if (current_)
                if (!reversed_)
                    current_ += 1;
                else
                    current_ -= 1;
        }
    };

    class Allocator;
public:
    Vector();
    explicit Vector(size_t size);
    explicit Vector(const std::initializer_list<T>& list);
    Vector(size_t size, const T& default_value);
    
    Vector(const Vector& other);
    Vector(Vector&& other);

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector operator=(Vector&& other);

    template <class _T>
    friend bool operator==(const Vector<_T>& v1, const Vector<_T>& v2);

    template <class _T>
    friend bool operator!=(const Vector<_T>& v1, const Vector<_T>& v2);

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

    Iterator begin() noexcept;
    Iterator end() noexcept;
    Iterator rbegin() noexcept;
    Iterator rend() noexcept;

    void resize(const size_t size);

    void reserve(const size_t capacity);
};

// ======================================
// ============ CONSTRUCTORS ============
// ======================================

template <class T>
Vector<T>::Vector(): size_(0), capacity_(0), memory_(nullptr) {std::cout << "C_default" << std::endl;}

template <class T>
Vector<T>::Vector(size_t size): size_(size), capacity_(size), memory_(new T[size]) { std::cout << "C_size" << std::endl; }

template <class T>
Vector<T>::Vector(const std::initializer_list<T>& list): size_(list.size()), capacity_(list.size())
{
    std::cout << "C_inic_list" << std::endl;

    memory_ = new T[capacity_];
    std::copy(list.begin(), list.end(), memory_);
}

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
Vector<T>::Vector(const Vector& other): size_(other.size_), capacity_(other.capacity_), memory_(new T[other.capacity_])
{
    std::cout << "C_copy" << std::endl;

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
bool operator==(const Vector<T>& v1, const Vector<T>& v2)
{
    std::cout << "operator==" << std::endl;

    if (v1.size_ != v2.size_)
        return false;
    
    for (size_t i = 0; i < v1.size_; i++)
    {
        if (v1.memory_[i] != v2.memory_[i])
            return false;
    }
    
    return true;
}

template <class T>
bool operator!=(const Vector<T>& v1, const Vector<T>& v2)
{
    return !operator==(v1, v2);
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
    size_ = 0;
}

template <class T>
void Vector<T>::resize(const size_t size)
{
    if (size > capacity_)
    {
        capacity_ = size;
        T* temp = new T[capacity_] {};
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
    return memory_[--size_];    
}

template <class T>
void Vector<T>::push_back(const T& el)
{
    std::cout << "referense push" << std::endl;
    if (capacity_ == size_)
    {
        if (capacity_ > 0)
            reserve(2 * capacity_);
        else
            reserve(1);
    }   
    memory_[size_++] = el;
}

template <class T>
void Vector<T>::push_back(T&& el)
{
    std::cout << "rvalue push" << std::endl;
    if (capacity_ == size_)
    {
        if (capacity_ > 0)
            reserve(2 * capacity_);
        else
            reserve(1);
    }   
    memory_[size_++] = std::move(el);
}
    
template <class T>
template <class... Args>
void Vector<T>::emplace_back(Args&&... args)
{
    if (capacity_ == size_)
    {
        if (capacity_ > 0)
            reserve(2 * capacity_);
        else
            reserve(1);

    }   
    memory_[size_++] = T(args...);
}

/////////////////////
///// ITERATORS /////
/////////////////////

template <class T>
typename Vector<T>::Iterator Vector<T>::begin() noexcept
{
    return Iterator(memory_);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::end() noexcept
{
    return Iterator(memory_ + size_);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::rbegin() noexcept
{
    return Iterator(memory_ + size_ - 1, true);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::rend() noexcept
{
    return Iterator(memory_ - 1, true);
}
