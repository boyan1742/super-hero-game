// Taken from: https://github.com/stoychoX/Object-oriented-programming-FMI/blob/main/Seminar/Seminar12/Unique.hpp

#pragma once

#include <utility>

template<typename T>
class UniquePtr
{
private:
    T *m_ptr;
public:
    UniquePtr(T *ptr);
    UniquePtr(const UniquePtr<T> &ptr) = delete;
    UniquePtr &operator=(const UniquePtr<T> &ptr) = delete;

    UniquePtr(UniquePtr<T> &&other) noexcept;
    UniquePtr &operator=(UniquePtr<T> &&other) noexcept;

    T *operator->();
    const T *operator->() const;
    T &operator*();
    const T &operator*() const;

    T *GetPointer();
    void Reset(T *ptr);
    T *Release(); // releases the ownership of the pointer

    ~UniquePtr();
private:
    void Move(UniquePtr<T> &&other) noexcept;
    void Free();
};

template<typename T>
UniquePtr<T>::UniquePtr(T *ptr)
{
    this->m_ptr = ptr;
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T> &&other) noexcept
{
    Move(std::move(other));
}

template<typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<T> &&other) noexcept
{
    if (this != &other)
    {
        Free();
        Move(std::move(other));
    }

    return *this;
}

template<typename T>
T *UniquePtr<T>::operator->()
{
    return m_ptr;
}

template<typename T>
const T *UniquePtr<T>::operator->() const
{
    return m_ptr;
}

template<typename T>
T &UniquePtr<T>::operator*()
{
    return *m_ptr;
}

template<typename T>
const T &UniquePtr<T>::operator*() const
{
    return *m_ptr;
}

template<typename T>
T *UniquePtr<T>::GetPointer()
{
    return this->m_ptr;
}

template<typename T>
void UniquePtr<T>::Reset(T *ptr)
{
    if (this->m_ptr == ptr)
        return;

    Free();
    this->m_ptr = ptr;
}

template<typename T>
T *UniquePtr<T>::Release()
{
    T *temp = m_ptr;
    m_ptr = nullptr;
    return temp;
}
template<typename T>
UniquePtr<T>::~UniquePtr()
{
    Free();
}

template<typename T>
void UniquePtr<T>::Move(UniquePtr<T> &&other) noexcept
{
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
}

template<typename T>
void UniquePtr<T>::Free()
{
    delete m_ptr;
}
