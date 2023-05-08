#include "InvalidCommandException.h"

char* InvalidCommandException::msg = "Bad command!";

char *InvalidCommandException::what()
{
    return InvalidCommandException::msg;
}
