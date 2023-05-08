#ifndef SUPERHEROES_ARRAY_H
#define SUPERHEROES_ARRAY_H

#include <initializer_list>
#include "../Exceptions/IndexOutOfRangeException.h"

template<typename T>
class Array
{
    static const int CAPACITY_PADDING = 10;

    T *m_items;
    size_t m_currentIndex;
    size_t m_capacity;
    bool m_frozen;

    void CopyFrom(const Array &other)
    {
        delete[] m_items;

        m_items = new T[other.m_capacity];
        for (int i = 0; i < other.m_capacity; ++i)
        {
            m_items[i] = other.m_items[i];
        }

        m_capacity = other.m_capacity;
        m_currentIndex = other.m_currentIndex;
        m_frozen = other.m_frozen;
    }
    void MoveFrom(Array &&other)
    {
        delete[] m_items;
        m_items = other.m_items;
        other.m_items = nullptr;
        m_currentIndex = other.m_currentIndex;
        m_capacity = other.m_capacity;
        m_frozen = other.m_frozen;

        other.m_capacity = other.m_currentIndex = 0;
        other.m_frozen = true;
    }
    void Free()
    {
        delete[] m_items;
        m_capacity = m_currentIndex = 0;
        m_frozen = true;
    }
    void Resize()
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
public:
    Array()
    {
        m_frozen = false;
        m_capacity = CAPACITY_PADDING;
        m_currentIndex = 0;
        m_items = new T[m_capacity];
    }
    explicit Array(size_t initialCapacity, bool freeze = false)
            : m_items(nullptr)
    {
        m_frozen = freeze;
        m_capacity = initialCapacity;
        m_currentIndex = 0;
        if (m_capacity > 0)
            m_items = new T[m_capacity];
    }
    Array(T *initialArray, size_t size, bool freeze = false)
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

    Array(std::initializer_list<T> list, bool freeze = false)
    {
        m_frozen = freeze;
        m_capacity = list.size() + (freeze ? 0 : CAPACITY_PADDING);

        m_items = new T[m_capacity];
        for (int i = 0; i < list.size(); ++i)
        {
            m_items[i] = data(list)[i];
        }
        m_currentIndex = list.size();
    }
    Array(const Array &copy)
            : m_capacity(0), m_currentIndex(0), m_items(nullptr)
    {
        CopyFrom(copy);
    }
    Array(Array &&other) noexcept
            : m_capacity(0), m_currentIndex(0), m_items(nullptr)
    {
        MoveFrom(std::move(other));
    }

    ~Array()
    {
        Free();
    }

    Array &operator=(const Array &other)
    {
        if (this != &other)
        {
            Free();
            CopyFrom(other);
        }
        return *this;
    }
    Array &operator=(Array &&other) noexcept
    {
        if (this != &other)
        {
            Free();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    const T& operator[](size_t index) const
    {
        if (index >= m_currentIndex)
            throw IndexOutOfRangeException("Index out of range exception!");

        return m_items[index];
    }

    T &operator[](size_t index)
    {
        if (index >= m_currentIndex)
            throw IndexOutOfRangeException("Index out of range exception!");

        return m_items[index];
    }

    void Add(const T &item)
    {
        if (m_currentIndex >= m_capacity)
        {
            if (m_frozen)
                return;

            Resize();
        }

        m_items[m_currentIndex++] = item;
    }
    void Add(T &&item)
    {
        if (m_currentIndex >= m_capacity)
        {
            if (m_frozen)
                return;

            Resize();
        }

        m_items[m_currentIndex++] = std::move(item);
    }

    void RemoveFromBack()
    {
        if (m_currentIndex == 0)
            return;

        m_currentIndex--;
    }

    size_t GetLength() const
    {
        return m_currentIndex;
    }
};


#endif //SUPERHEROES_ARRAY_H
