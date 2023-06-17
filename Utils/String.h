#ifndef ZAD1_STRING_H
#define ZAD1_STRING_H

#include <fstream>
#include "Array.hpp"

class String
{
    // Taken and modified from: https://pvs-studio.com/en/blog/terms/6658/
    union
    {
        struct
        {
            char *m_data;
            size_t m_size;
        } m_heapBuffer;

        char m_stackBuffer[sizeof(m_heapBuffer)];
    } m_data;

    static const int CAPACITY_PADDING = sizeof(m_data) - 1;
    size_t m_capacity;

    void ClearStaticArray();
    void CopyFrom(const String &other);
    void MoveFrom(String&& other);
    void Free();
    void Resize(size_t newCapacity);

    void SetData(const char *str);

    explicit String(size_t capacity);
public:
    String();
    String(const char *str);
    String(const String &copy);
    String(String&& other);
    ~String();

    String &operator=(const String &other);
    String& operator=(String&& other);

    char operator[](size_t idx) const;
    char &operator[](size_t idx);

    size_t GetCapacity() const;
    size_t GetSize() const;
    bool IsSSO() const;

    void Append(const String &str);
    void Append(String &&str);
    void Append(char ch);
    void Append(uint64_t number);
    void Append(int64_t number);
    void Append(uint32_t number);
    void Append(int32_t number);
    void Append(uint16_t number);
    void Append(int16_t number);
    void Append(uint8_t number);
    void Append(int8_t number);
    void Append(double number);

    bool Contains(char ch) const;
    size_t AmountOf(char ch) const;
    Array<String> Split(char ch) const;

    bool operator==(const String &rhs) const;
    bool operator!=(const String &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const String &str);
    friend std::istream &operator>>(std::istream &is, String &str);

    String &operator+=(const String &rhs);
    String &operator+=(uint64_t rhs);
    String &operator+=(int64_t rhs);
    String &operator+=(uint32_t rhs);
    String &operator+=(int32_t rhs);
    String &operator+=(uint16_t rhs);
    String &operator+=(int16_t rhs);
    String &operator+=(uint8_t rhs);
    String &operator+=(int8_t rhs);
    String &operator+=(double rhs);

    bool IsEmpty() const;
    void Clear();
    const char *c_str() const;

    static void GetLine(std::istream &istream, String *pString);
    static void ReadExact(std::ifstream &ifstream, size_t amount, String *pString);

    void ToLower();
    void ToUpper();
    bool EndsWith(const String& string) const;
};

std::ostream &operator<<(std::ostream &os, const String &str);
std::istream &operator>>(std::istream &is, String &str);

String operator+(const String &lhs, const String &rhs);
String operator+(const String &lhs, uint64_t rhs);
String operator+(const String &lhs, int64_t rhs);
String operator+(const String &lhs, uint32_t rhs);
String operator+(const String &lhs, int32_t rhs);
String operator+(const String &lhs, uint16_t rhs);
String operator+(const String &lhs, int16_t rhs);
String operator+(const String &lhs, uint8_t rhs);
String operator+(const String &lhs, int8_t rhs);
String operator+(const String &lhs, double rhs);


#endif //ZAD1_STRING_H
