#pragma once

#include <initializer_list>
#include <new>

template <class T>
class Allocator
{
public:
    Allocator() {}
    Allocator(const Allocator&) = delete;
    Allocator(Allocator&&) = delete;

    ~Allocator() {}

    Allocator& operator=(const Allocator& other) = default;
    Allocator& operator=(Allocator&&) = delete;

    T* allocate(const size_t size)
    {
        return static_cast<T*>(operator new[] (size * sizeof(T)));
    }

    void deallocate(T* obj, const size_t size)
    {
        operator delete[] (obj, size * sizeof(T));
    }
    
    template <class... Args>
    void construct(T* obj, Args&&... args)
    {
        new (obj) T(std::forward<Args>(args)...);
    }

    void destroy(T* obj)
    {
        (*obj).~T();
    }
};

template <class T>
class Vector
{
    size_t size_;
    size_t capacity_;
    T* memory_;

    Allocator<T> allocator_;

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
public:
    Vector();
    explicit Vector(size_t size);
    explicit Vector(const std::initializer_list<T>& list);
    Vector(size_t size, const T& default_value);
    
    Vector(const Vector& other);
    Vector(Vector&& other);

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);

    template <class _T>
    friend bool operator==(const Vector<_T>& v1, const Vector<_T>& v2);

    template <class _T>
    friend bool operator!=(const Vector<_T>& v1, const Vector<_T>& v2);

    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    void push_back(const T& el);
    void push_back(T&& el);

    void pop_back();

    template <class... Args>
    void emplace_back(Args&&... args);

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
Vector<T>::Vector(): size_(0), capacity_(0), memory_(nullptr) {}

// T must have default constructor
template <class T>
Vector<T>::Vector(size_t size): size_(size), capacity_(size)
{
    memory_ = allocator_.allocate(size);
    for (size_t i = 0; i < size; i++)
    {
        allocator_.construct(memory_ + i);
    }
    
}

template <class T>
Vector<T>::Vector(const std::initializer_list<T>& list): size_(list.size()), capacity_(list.size())
{
    memory_ = allocator_.allocate(size_);
    
    size_t i = 0;
    for (const auto& element : list)
    {
        allocator_.construct(memory_ + i, element);
        i++;
    }
}

template <class T>
Vector<T>::Vector(size_t size, const T& default_value): size_(size), capacity_(size)
{
    memory_ = allocator_.allocate(capacity_);
    for (size_t i = 0; i < size_; i++)
    {
        allocator_.construct(memory_ + i, default_value);
    }
}

template <class T>
Vector<T>::Vector(const Vector& other): size_(other.size_), capacity_(other.capacity_)
{
    memory_ = allocator_.allocate(other.capacity_);
    for (size_t i = 0; i < size_; i++)
    {
        allocator_.construct(memory_ + i, *(other.memory_ + i));
    }
}

template <class T>
Vector<T>::Vector(Vector&& other): size_(other.size_), capacity_(other.capacity_), memory_(other.memory_)
{
    other.size_ = 0;
    other.capacity_ = 0;
    other.memory_ = nullptr;
}

template <class T>
Vector<T>::~Vector()
{
    for (size_t i = 0; i < size_; i++)
    {
        allocator_.destroy(memory_ + i);
    }
    allocator_.deallocate(memory_, capacity_);
}

// ======================================
// ============= OPERATORS ==============
// ======================================

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    // self assign
    if ((*this).memory_ == other.memory_)
        return *this;

    // we can no allocate new memory if it not needed
    if (other.size_ > capacity_)
    {

        T* temp = allocator_.allocate(other.size_);

        // delete memory_
        for (size_t i = 0; i < size_; i++)
        {
            allocator_.destroy(memory_ + i);
        }
        allocator_.deallocate(memory_, capacity_);

        memory_ = temp;
        
        capacity_ = other.size_;
    }
    else
    {
        for (size_t i = 0; i < size_; i++)
        {
            allocator_.destroy(memory_ + i);
        }
    }

    // copy values from other to this
    for (size_t i = 0; i < other.size_; i++)
    {
        allocator_.construct(memory_ + i, *(other.memory_ + i));
    }
    
    size_ = other.size_;

    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector&& other)
{
    // self assign
    if ((*this).memory_ == other.memory_)
        return *this;

    // delete memory_
    for (size_t i = 0; i < size_; i++)
    {
        allocator_.destroy(memory_ + i);
    }
    allocator_.deallocate(memory_, capacity_);
    
    
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
    for (size_t i = 0; i < size_; i++)
    {
        allocator_.destroy(memory_ + i);
    }
    size_ = 0;
}

template <class T>
void Vector<T>::resize(const size_t size)
{
    if (size > size_)
    {
        if (size > capacity_)
        {
            // copy  memory_ to temp
            T* temp = allocator_.allocate(size);
            for (size_t i = 0; i < size; i++)
            {
                if (i < size_)
                    allocator_.construct(temp + i, *(memory_ + i));
                else
                    allocator_.construct(temp + i);
            }
            
            // delete memory_
            for (size_t i = 0; i < size_; i++)
            {
                allocator_.destroy(memory_ + i);
            }
            allocator_.deallocate(memory_ ,capacity_);
            
            memory_ = temp;
            capacity_ = size;
        }
        else
        {
            for (size_t i = size_; i < size; i++)
            {
                allocator_.construct(memory_ + i);
            }
        }
    }
    else
    {
        // delete tail
        for (size_t i = size; i < size_; i++)
        {
            allocator_.destroy(memory_ + i);
        }
    }
    
    size_ = size;
}

template <class T>
void Vector<T>::reserve(const size_t capacity)
{
    if (capacity > capacity_)
    {
        T* temp = allocator_.allocate(capacity);
        
        // copy
        for (size_t i = 0; i < size_; i++)
        {
            allocator_.construct(temp + i, *(memory_ + i));
        }
        
        for (size_t i = 0; i < size_; i++)
        {
            allocator_.destroy(memory_ + i);
        }
        allocator_.deallocate(memory_, capacity_);
        
        memory_ = temp;
        capacity_ = capacity;
    }
}

template <class T>
void Vector<T>::pop_back()
{
    size_--;

    allocator_.destroy(memory_ + size_);
}

template <class T>
void Vector<T>::push_back(const T& el)
{
    if (capacity_ == size_)
    {
        if (capacity_ > 0)
            reserve(2 * capacity_);
        else
            reserve(1);
    }   

    allocator_.construct(memory_ + size_, el);
    size_++;
}

template <class T>
void Vector<T>::push_back(T&& el)
{
    if (capacity_ == size_)
    {
        if (capacity_ > 0)
            reserve(2 * capacity_);
        else
            reserve(1);
    }   
    
    allocator_.construct(memory_ + size_, el);
    size_++;
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

    allocator_.construct(memory_ + size_, args...);
    size_++;
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
