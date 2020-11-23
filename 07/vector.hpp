#pragma once

template <class T, class Iterator, class Allocator>
class Vector
{
    size_t size_;
    size_t capacity_;
    T* memory_;
public:
    Vector(T* array, size_t size);
    explicit Vector(size_t size);
    Vector(size_t size, const T& default_value);

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

    bool empty() { return size_ == 0; } const noexcept;

    size_t size() { return size_; } const noexcept;
    size_t capacity() { return capacity_; } const noexcept;

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