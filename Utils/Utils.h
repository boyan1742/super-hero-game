#ifndef SUPERHEROES_UTILS_H
#define SUPERHEROES_UTILS_H

#include "String.h"

class Utils
{
public:
    static String HashString(const String& normal);
    static bool ValidateUsername(const String& username);
    static bool ValidatePassword(const String& password);
    static bool ValidateEmail(const String& email);
    static bool ValidateName(const String& name);
};


#endif //SUPERHEROES_UTILS_H
