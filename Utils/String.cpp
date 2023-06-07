#include "String.h"
#include <cstring>
#include <stdexcept>

String::String(size_t capacity)
        : m_data()
{
    m_capacity = capacity;
    if (!IsSSO())
    {
        m_data.m_heapBuffer.m_data = new char[m_capacity]{'\0'};
        m_data.m_heapBuffer.m_size = 0;
    } else
    {
        ClearStaticArray();
    }
}

String::String()
        : String(CAPACITY_PADDING)
{

}
String::String(const char *str)
        : String(strlen(str) <= CAPACITY_PADDING ? CAPACITY_PADDING : strlen(str) + CAPACITY_PADDING)
{
    if (!IsSSO()) // heap
    {
        strcpy_s(m_data.m_heapBuffer.m_data, m_capacity, str);
        m_data.m_heapBuffer.m_size = strlen(str);
    } else // stack
        strcpy_s(m_data.m_stackBuffer, str);
}
String::String(const String &copy)
        : m_data(), m_capacity(0)
{
    CopyFrom(copy);
}

String::String(String &&other)
{
    MoveFrom(std::move(other));
}

String::~String()
{
    Free();
}

String &String::operator=(String &&other)
{
    if (this != &other)
    {
        Free();
        MoveFrom(std::move(other));
    }
    return *this;
}

String &String::operator=(const String &other)
{
    if (this != &other)
    {
        Free();
        CopyFrom(other);
    }
    return *this;
}
void String::CopyFrom(const String &other)
{
    m_capacity = other.m_capacity;
    if (!IsSSO())
    {
        m_data.m_heapBuffer.m_data = new char[other.m_capacity];
        strcpy_s(m_data.m_heapBuffer.m_data, m_capacity, other.m_data.m_heapBuffer.m_data);
        m_data.m_heapBuffer.m_size = other.m_data.m_heapBuffer.m_size;
    } else
    {
        strcpy_s(m_data.m_stackBuffer, other.m_data.m_stackBuffer);
    }
}

void String::MoveFrom(String &&other)
{
    m_data.m_heapBuffer.m_data = other.m_data.m_heapBuffer.m_data;
    m_data.m_heapBuffer.m_size = other.m_data.m_heapBuffer.m_size;

    m_capacity = other.m_capacity;
    other.m_capacity = 0;
    other.m_data.m_heapBuffer.m_data = nullptr;
    other.m_data.m_heapBuffer.m_size = 0;
}

void String::Free()
{
    if (!IsSSO())
    {
        delete[] m_data.m_heapBuffer.m_data;
        m_data.m_heapBuffer.m_data = nullptr;
        m_data.m_heapBuffer.m_size = 0;
    } else
        ClearStaticArray();
}
void String::ClearStaticArray()
{
    for (int i = 0; i < sizeof(m_data.m_heapBuffer); i++)
        m_data.m_stackBuffer[i] = '\0';
}
void String::Resize(size_t newCapacity)
{
    size_t cap = newCapacity + CAPACITY_PADDING;
    char *temp = new char[cap];
    for (int i = 0; i < cap; ++i)
    {
        temp[i] = '\0';
    }

    if (!IsSSO())
    {
        strcpy_s(temp, cap, m_data.m_heapBuffer.m_data);
        delete[] m_data.m_heapBuffer.m_data;
    } else
    {
        strcpy_s(temp, cap, m_data.m_stackBuffer);
        ClearStaticArray();
    }

    m_data.m_heapBuffer.m_data = temp;
    m_data.m_heapBuffer.m_size = strlen(temp);
    m_capacity = cap;
}
char String::operator[](size_t idx) const
{
    if (!IsSSO()) // heap
    {
        if (idx >= m_data.m_heapBuffer.m_size)
            throw std::runtime_error("Index out of range.");

        return m_data.m_heapBuffer.m_data[idx];
    }

    if (idx >= sizeof(m_data.m_heapBuffer))
        throw std::runtime_error("Index out of range.");

    return m_data.m_stackBuffer[idx];
}
char &String::operator[](size_t idx)
{
    if (!IsSSO()) // heap
    {
        if (idx >= m_data.m_heapBuffer.m_size)
            throw std::runtime_error("Index out of range.");

        return m_data.m_heapBuffer.m_data[idx];
    }

    if (idx >= sizeof(m_data.m_heapBuffer))
        throw std::runtime_error("Index out of range.");

    return m_data.m_stackBuffer[idx];
}
void String::Append(const String &str)
{
    if (str.IsEmpty())
        return;

    size_t len = str.GetSize();
    if (len + GetSize() >= m_capacity)
    {
        //resize
        Resize(len + GetSize());
    }

    size_t s = len + GetSize();
    strcat_s(!IsSSO() ?
             m_data.m_heapBuffer.m_data : m_data.m_stackBuffer,
             m_capacity,
             !str.IsSSO() ?
             str.m_data.m_heapBuffer.m_data : str.m_data.m_stackBuffer);
    char *b = !IsSSO() ? m_data.m_heapBuffer.m_data : m_data.m_stackBuffer;
    b[s] = '\0';

    if (!IsSSO())
        m_data.m_heapBuffer.m_size = s;

}
void String::Append(char ch)
{
    if (GetSize() + 1 >= m_capacity)
    {
        //resize
        Resize(CAPACITY_PADDING + GetSize()); // will resize by 2*CAPACITY_PADDING + Size
    }

    if (IsSSO())
        m_data.m_stackBuffer[GetSize()] = ch;
    else
        m_data.m_heapBuffer.m_data[m_data.m_heapBuffer.m_size++] = ch;

}
void String::Append(uint64_t number)
{
    char buff[16];
    sprintf_s(buff, "%llu", number);
    Append(buff);
}
void String::Append(int64_t number)
{
    char buff[16];
    sprintf_s(buff, "%lld", number);
    Append(buff);
}
void String::Append(uint32_t number)
{
    char buff[16];
    sprintf_s(buff, "%u", number);
    Append(buff);
}
void String::Append(int32_t number)
{
    char buff[16];
    sprintf_s(buff, "%d", number);
    Append(buff);
}
void String::Append(uint16_t number)
{
    char buff[16];
    sprintf_s(buff, "%u", number);
    Append(buff);
}
void String::Append(int16_t number)
{
    char buff[16];
    sprintf_s(buff, "%d", number);
    Append(buff);
}
void String::Append(uint8_t number)
{
    char buff[16];
    sprintf_s(buff, "%u", number);
    Append(buff);
}
void String::Append(int8_t number)
{
    char buff[16];
    sprintf_s(buff, "%d", number);
    Append(buff);
}
void String::Append(double number)
{
    char buff[16];
    sprintf_s(buff, "%f", number);
    Append(buff);
}
size_t String::GetCapacity() const
{
    return m_capacity;
}
size_t String::GetSize() const
{
    return !IsSSO() ?
           m_data.m_heapBuffer.m_size :
           strlen(m_data.m_stackBuffer);
}
std::ostream &operator<<(std::ostream &os, const String &str)
{
    return os << (!str.IsSSO() ? str.m_data.m_heapBuffer.m_data : str.m_data.m_stackBuffer);
}
std::istream &operator>>(std::istream &is, String &str)
{
    char buff[1024];
    is >> buff;
    str = buff;
    return is;
}
String operator+(const String &lhs, const String &rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
bool String::IsSSO() const
{
    return m_capacity < sizeof(m_data.m_heapBuffer);
}
bool String::IsEmpty() const
{
    return IsSSO() ? m_data.m_stackBuffer[0] == '\0' : m_data.m_heapBuffer.m_data[0] == '\0';
}
bool String::operator==(const String &rhs) const
{
    return strcmp(
            IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data,
            rhs.IsSSO() ? rhs.m_data.m_stackBuffer : rhs.m_data.m_heapBuffer.m_data
    ) == 0;
}
bool String::operator!=(const String &rhs) const
{
    return !(*this == rhs);
}
Array<String> String::Split(char ch) const
{
    if (!Contains(ch))
    {
        Array<String> arr = Array<String>(1);
        arr.Add(*this);
        return arr;
    }

    const char *buffer = IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;

    Array<String> arr(AmountOf(ch) + 1 - (buffer[0] == ch ? 1 : 0), true);
    String temp;

    size_t size = GetSize();

    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == ch)
        {
            if (temp.IsEmpty())
                continue;

            arr.Add(std::move(temp));
            temp = String();
        } else
        {
            temp.Append(buffer[i]);
        }
    }

    if (!temp.IsEmpty())
    {
        temp.Append('\0');
        arr.Add(std::move(temp));
    }

    return arr;
}
bool String::Contains(char ch) const
{
    size_t size = GetSize();
    const char *buffer = IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;

    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == ch)
            return true;
    }

    return false;
}
size_t String::AmountOf(char ch) const
{
    if (!Contains(ch))
        return 0;

    size_t amount = 0;
    size_t size = GetSize();
    const char *buffer = IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;

    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == ch)
            amount++;
    }

    return amount;
}
void String::Clear()
{
    if (!IsSSO())
        delete[] m_data.m_heapBuffer.m_data;

    ClearStaticArray();
    m_capacity = CAPACITY_PADDING;
}
const char *String::c_str() const
{
    return IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;
}


void String::GetLine(std::istream &istream, String *pString)
{
    if (!pString)
        return;

    char buffer[1024];
    istream.getline(buffer, 1024, '\n');
    pString->SetData(buffer);
}

void String::SetData(const char *str)
{
    if (!IsSSO())
    {
        delete[] m_data.m_heapBuffer.m_data;
        m_data.m_heapBuffer.m_size = 0;
    }

    size_t len = strlen(str);
    if (len > sizeof(m_data.m_heapBuffer))
    {
        m_capacity = len + 1 + CAPACITY_PADDING;
        m_data.m_heapBuffer.m_data = new char[len + 1 + CAPACITY_PADDING];
        strcpy_s(m_data.m_heapBuffer.m_data, m_capacity, str);
        m_data.m_heapBuffer.m_size = len;
    } else
    {
        strcpy_s(m_data.m_stackBuffer, str);
        m_capacity = strlen(str);
    }
}
void String::ToLower()
{
    char *buffer = IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;

    for (int i = 0; i < GetSize(); ++i)
    {
        if (buffer[i] >= 'A' && buffer[i] <= 'Z')
            buffer[i] ^= 0x20; // flip the 5th bit to switch between cases.
    }
}
void String::ToUpper()
{
    char *buffer = IsSSO() ? m_data.m_stackBuffer : m_data.m_heapBuffer.m_data;

    for (int i = 0; i < GetSize(); ++i)
    {
        if (buffer[i] >= 'a' && buffer[i] <= 'z')
            buffer[i] ^= 0x20; // flip the 5th bit to switch between cases.
    }
}
String &String::operator+=(const String &rhs)
{
    Append(rhs);
    return *this;
}
void String::ReadExact(std::ifstream &ifstream, size_t amount, String *pString)
{
    char *buffer = new char[amount];
    ifstream.read(buffer, amount);
    pString->SetData(buffer);
}
String &String::operator+=(uint64_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(int64_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(uint32_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(int32_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(uint16_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(int16_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(uint8_t rhs)
{
    Append(rhs);
    return *this;
}

String &String::operator+=(int8_t rhs)
{
    Append(rhs);
    return *this;
}

String operator+(const String &lhs, uint64_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, int64_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, uint32_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, int32_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, uint16_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, int16_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, uint8_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, int8_t rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
String operator+(const String &lhs, double rhs)
{
    String copy = lhs;
    copy += rhs;
    return copy;
}
void String::Append(String &&str)
{
    if (str.IsEmpty())
        return;

    size_t len = str.GetSize();
    if (len + GetSize() >= m_capacity)
    {
        //resize
        Resize(len + GetSize());
    }

    size_t s = len + GetSize();
    strcat_s(!IsSSO() ?
             m_data.m_heapBuffer.m_data : m_data.m_stackBuffer,
             m_capacity,
             !str.IsSSO() ?
             str.m_data.m_heapBuffer.m_data : str.m_data.m_stackBuffer);
    char *b = !IsSSO() ? m_data.m_heapBuffer.m_data : m_data.m_stackBuffer;
    b[s] = '\0';

    if (!IsSSO())
        m_data.m_heapBuffer.m_size = s;

    str.Free();
}
String &String::operator+=(double rhs)
{
    Append(rhs);
    return *this;
}




