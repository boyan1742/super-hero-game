#include "Person.h"

Person::Person(const String &firstName, const String &lastName, const String &email, const String &username,
               const String &password)
        : m_firstName(firstName), m_lastName(lastName), m_email(email), m_username(username), m_password(password)
{

}
const String &Person::GetFirstName() const
{
    return m_firstName;
}
const String &Person::GetLastName() const
{
    return m_lastName;
}
const String &Person::GetUsername() const
{
    return m_username;
}
const String &Person::GetEmail() const
{
    return m_email;
}
const String &Person::GetPassword() const
{
    return m_password;
}
