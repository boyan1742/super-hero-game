#ifndef SUPERHEROES_USER_H
#define SUPERHEROES_USER_H

#include "../Utils/String.h"

enum class UserType
{
    Player,
    Admin
};

class User
{
    String m_firstName = "";
    String m_lastName = "";
    String m_email = "";
    String m_username = "";
    String m_password = "";

    User() = default;
public:
    User(const String &firstName, const String &lastName, const String &email, const String &username,
         const String &password);
    virtual ~User() = default;

    const String &GetFirstName() const;
    const String &GetLastName() const;
    const String &GetUsername() const;

    virtual UserType GetUserType() = 0;

protected:
    const String &GetEmail() const;
    const String &GetPassword() const;
};


#endif //SUPERHEROES_USER_H
