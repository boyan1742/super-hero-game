#ifndef SUPERHEROES_INVALIDCOMMANDEXCEPTION_H
#define SUPERHEROES_INVALIDCOMMANDEXCEPTION_H

#include <stdexcept>
#include "../Utils/String.h"

class InvalidCommandException : std::exception
{
    static char* msg;
public:
    InvalidCommandException() = default;
    char* what();
};


#endif //SUPERHEROES_INVALIDCOMMANDEXCEPTION_H
