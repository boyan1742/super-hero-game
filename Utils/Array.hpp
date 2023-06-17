#ifndef SUPERHEROES_ARRAY_HPP
#define SUPERHEROES_ARRAY_HPP

#include <initializer_list>
#include "../Exceptions/IndexOutOfRangeException.h"

template<typename T>
class Array
{
    static const int CAPACITY_PADDING = 10;

    T *m_items = nullptr;
    size_t m_currentIndex;
    size_t m_capacity;
    bool m_frozen;

    void CopyFrom(const Array &other);
    void MoveFrom(Array &&other);
    void Free();
    void Resize();
public:
    Array();
    explicit Array(size_t initialCapacity, bool freeze = false);
    Array(T *initialArray, size_t size, bool freeze = false);

    Array(const Array &copy);
    Array(Array &&other) noexcept;

    ~Array();

    Array &operator=(const Array &other);
    Array &operator=(Array &&other) noexcept;

    const T &operator[](size_t index) const;

    T &operator[](size_t index);

    void Add(const T &item);
    void Add(T &&item);

    void RemoveFromBack();

    size_t GetLength() const;

    bool IsEmpty() const;

    bool Contains(const T &obj) const;
};

////////////////////////////////////////

template<typename T>
Array<T>::Array()
{
    m_frozen = false;
    m_capacity = CAPACITY_PADDING;
    m_currentIndex = 0;
    m_items = new T[m_capacity];
}
template<typename T>
Array<T>::Array(size_t initialCapacity, bool freeze)
        : m_items(nullptr)
{
    m_frozen = freeze;
    m_capacity = initialCapacity;
    m_currentIndex = 0;
    if (m_capacity > 0)
        m_items = new T[m_capacity];
}
template<typename T>
Array<T>::Array(T *initialArray, size_t size, bool freeze)
        : m_capacity(0), m_currentIndex(0), m_items(nullptr)
{
    if (!initialArray)
    {
        m_frozen = false;
        m_capacity = CAPACITY_PADDING;
        m_items = new T[m_capacity];
        return;
    }

    m_frozen = freeze;

    m_capacity = size + CAPACITY_PADDING;
    m_items = new T[m_capacity];
    for (int i = 0; i < size; ++i)
    {
        m_items[i] = initialArray[i];
    }

    m_currentIndex = size;
}
template<typename T>
Array<T>::Array(const Array &copy)
        : m_capacity(0), m_currentIndex(0), m_items(nullptr), m_frozen(false)
{
    CopyFrom(copy);
}
template<typename T>
Array<T>::Array(Array &&other) noexcept
        : m_capacity(0), m_currentIndex(0), m_items(nullptr), m_frozen(false)
{
    MoveFrom(std::move(other));
}
template<typename T>
Array<T>::~Array()
{
    Free();
}

////////////////////////////////////////

template<typename T>
Array<T> &Array<T>::operator=(Array &&other) noexcept
{
    if (this != &other)
    {
        Free();
        MoveFrom(std::move(other));
    }
    return *this;
}
template<typename T>
Array<T> &Array<T>::operator=(const Array &other)
{
    if (this != &other)
    {
        Free();
        CopyFrom(other);
    }
    return *this;
}

template<typename T>
T &Array<T>::operator[](size_t index)
{
    if (index >= m_currentIndex)
        throw IndexOutOfRangeException("Index out of range exception!");

    return m_items[index];
}
template<typename T>
const T &Array<T>::operator[](size_t index) const
{
    if (index >= m_currentIndex)
        throw IndexOutOfRangeException("Index out of range exception!");

    return m_items[index];
}

////////////////////////////////////////

template<typename T>
void Array<T>::Add(const T &item)
{
    if (m_currentIndex >= m_capacity)
    {
        if (m_frozen)
            return;

        Resize();
    }

    m_items[m_currentIndex++] = item;
}
template<typename T>
void Array<T>::Add(T &&item)
{
    if (m_currentIndex >= m_capacity)
    {
        if (m_frozen)
            return;

        Resize();
    }

    m_items[m_currentIndex++] = std::move(item);
}
template<typename T>
void Array<T>::RemoveFromBack()
{
    if (m_currentIndex == 0)
        return;

    m_currentIndex--;
}

////////////////////////////////////////

template<typename T>
size_t Array<T>::GetLength() const
{
    return m_currentIndex;
}
template<typename T>
bool Array<T>::IsEmpty() const
{
    return m_currentIndex == 0;
}
template<typename T>
bool Array<T>::Contains(const T &obj) const
{
    for (int i = 0; i < m_currentIndex; ++i)
    {
        if (m_items[i] == obj)
            return true;
    }

    return false;
}

////////////////////////////////////////

template<typename T>
void Array<T>::Resize()
{
    if (m_frozen)
        return;

    if (!m_items)
    {
        m_capacity += CAPACITY_PADDING;
        m_items = new T[m_capacity];
        m_currentIndex = 0;
        return;
    }

    T *temp = new T[m_capacity + CAPACITY_PADDING];
    for (int i = 0; i < m_currentIndex; ++i)
    {
        temp[i] = m_items[i];
    }

    delete[] m_items;
    m_items = temp;

    m_capacity += CAPACITY_PADDING;
}
template<typename T>
void Array<T>::Free()
{
    delete[] m_items;
    m_items = nullptr;
    m_capacity = m_currentIndex = 0;
    m_frozen = true;
}
template<typename T>
void Array<T>::MoveFrom(Array &&other)
{
    m_items = other.m_items;
    other.m_items = nullptr;

    m_currentIndex = other.m_currentIndex;
    m_capacity = other.m_capacity;
    m_frozen = other.m_frozen;

    other.m_capacity = other.m_currentIndex = 0;
    other.m_frozen = true;
}
template<typename T>
void Array<T>::CopyFrom(const Array &other)
{
    m_items = new T[other.m_capacity];
    for (int i = 0; i < other.m_capacity; ++i)
    {
        m_items[i] = other.m_items[i];
    }

    m_capacity = other.m_capacity;
    m_currentIndex = other.m_currentIndex;
    m_frozen = other.m_frozen;
}


#endif //SUPERHEROES_ARRAY_HPP
