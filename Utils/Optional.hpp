#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <utility>

template<typename T>
class Optional
{
    T *m_data;

    void CopyForm(const Optional &other);
    void MoveForm(Optional &&other);
    void Free();
public:
    Optional();
    explicit Optional(const T &value);
    explicit Optional(T &&value);

    Optional(const Optional &copy);
    Optional(Optional &&other) noexcept;

    ~Optional();

    Optional &operator=(const Optional &other);
    Optional &operator=(Optional &&other) noexcept;

    const T &GetData() const;
    bool HasValue() const;
};

////////////////////////////////////////////////////////////

template<typename T>
void Optional<T>::CopyForm(const Optional &other)
{
    m_data = new T(*other.m_data);
}
template<typename T>
void Optional<T>::MoveForm(Optional &&other)
{
    m_data = other.m_data;
    other.m_data = nullptr;
}
template<typename T>
void Optional<T>::Free()
{
    delete m_data;
}

////////////////////////////////////////////////////////////

template<typename T>
Optional<T>::Optional()
{
    m_data = nullptr;
}

template<typename T>
Optional<T>::Optional(const T &value)
{
    m_data = new T(value);
}

template<typename T>
Optional<T>::Optional(T &&value)
{
    m_data = new T(std::move(value));
}
template<typename T>
Optional<T>::Optional(const Optional &copy)
{
    CopyFrom(copy);
}
template<typename T>
Optional<T>::Optional(Optional &&other) noexcept
{
    MoveForm(std::move(other));
}
template<typename T>
Optional<T>::~Optional()
{
    Free();
}
template<typename T>
Optional<T> &Optional<T>::operator=(const Optional &other)
{
    if (this != &other)
    {
        Free();
        CopyForm(other);
    }
    return *this;
}
template<typename T>
Optional<T> &Optional<T>::operator=(Optional &&other) noexcept
{
    if (this != &other)
    {
        Free();
        MoveForm(std::move(other));
    }
    return *this;
}
template<typename T>
const T &Optional<T>::GetData() const
{
    return *m_data;
}
template<typename T>
bool Optional<T>::HasValue() const
{
    return m_data != nullptr;
}
#endif
