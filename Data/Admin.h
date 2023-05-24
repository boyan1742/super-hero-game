#ifndef SUPERHEROES_ADMIN_H
#define SUPERHEROES_ADMIN_H

#include "User.h"

class Admin : public User
{
public:
    Admin(const String &firstName, const String &lastName, const String &email, const String &username,
          const String &password);

    UserType GetUserType() override;
};


#endif //SUPERHEROES_ADMIN_H
