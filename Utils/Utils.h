#ifndef SUPERHEROES_UTILS_H
#define SUPERHEROES_UTILS_H

#include "String.h"
#include "../Data/SuperheroMode.h"
#include "../Data/SuperheroElements.h"

class Utils
{
public:
    static String HashString(const String& normal);
    static bool ValidateUsername(const String& username);
    static bool ValidatePassword(const String& password);
    static bool ValidateEmail(const String& email);
    static bool ValidateName(const String& name);

    static bool ParseSuperHeroMode(const String &str, SuperheroMode *mode);
    static bool ParseSuperHeroElement(const String &str, SuperheroElements *element);
};


#endif //SUPERHEROES_UTILS_H
