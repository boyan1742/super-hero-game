#ifndef ZAD1_STRING_H
#define ZAD1_STRING_H

#include <fstream>


class String
{
    // Taken and modified from: https://pvs-studio.com/en/blog/terms/6658/
    union
    {
        struct
        {
            char* m_data;
            size_t m_size;
        } m_heapBuffer;

        char m_stackBuffer[sizeof(m_heapBuffer)];
    } m_data;

    static const int CAPACITY_PADDING = sizeof(m_data) - 1;
    size_t m_capacity;

    void ClearStaticArray();
    void CopyFrom(const String& other);
    void Free();
    void Resize(size_t newCapacity);

    explicit String(size_t capacity);
public:
    String();
    String(const char* str);
    String(const String& copy);
    ~String();

    String& operator=(const String& other);

    char operator[](size_t idx) const;
    char& operator[](size_t idx);

    size_t GetCapacity() const;
    size_t GetSize() const;
    bool IsSSO() const;

    void Append(const String& str);
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

    friend std::ostream& operator<<(std::ostream& os, const String& str);
    friend std::istream& operator>>(std::istream& is, String& str);

    bool IsEmpty() const;
};

std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& operator>>(std::istream& is, String& str);

#endif //ZAD1_STRING_H
