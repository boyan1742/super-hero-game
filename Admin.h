#ifndef SUPERHEROES_ADMIN_H
#define SUPERHEROES_ADMIN_H

#include "Person.h"

class Admin : Person
{
public:
    Admin(const String &firstName, const String &lastName, const String &email, const String &username,
          const String &password);
};


#endif //SUPERHEROES_ADMIN_H
