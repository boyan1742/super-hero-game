#include "InvalidCommandException.h"

static const char* m_msg = "Bad command!";
const char *InvalidCommandException::what() const
{
    return m_msg;
}
