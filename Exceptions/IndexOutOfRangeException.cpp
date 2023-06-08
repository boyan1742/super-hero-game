#include "IndexOutOfRangeException.h"
#include <cstring>

IndexOutOfRangeException::IndexOutOfRangeException()
{
    m_msg = new char[1];
    m_msg[0] = '\0';
}
IndexOutOfRangeException::IndexOutOfRangeException(const char *msg)
{
    m_msg = new char[strlen(msg) + 1];
    strcpy_s(m_msg, strlen(msg) + 1, msg);
}
const char *IndexOutOfRangeException::what() const
{
    return m_msg;
}
