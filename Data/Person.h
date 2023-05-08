#ifndef SUPERHEROES_PERSON_H
#define SUPERHEROES_PERSON_H

#include "../Utils/String.h"

class Person
{
    String m_firstName = "";
    String m_lastName = "";
    String m_email = "";
    String m_username = "";
    String m_password = "";

    Person() = default;
public:
    Person(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password);

    const String &GetFirstName() const;
    const String &GetLastName() const;
    const String &GetUsername() const;

protected:
    const String &GetEmail() const;
    const String &GetPassword() const;
};


#endif //SUPERHEROES_PERSON_H
