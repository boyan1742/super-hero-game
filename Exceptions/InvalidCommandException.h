#ifndef SUPERHEROES_INVALIDCOMMANDEXCEPTION_H
#define SUPERHEROES_INVALIDCOMMANDEXCEPTION_H

#include <stdexcept>
#include "../Utils/String.h"

class InvalidCommandException : public std::exception
{
public:
    InvalidCommandException() = default;

    const char *what() const override;
};


#endif //SUPERHEROES_INVALIDCOMMANDEXCEPTION_H
