#include "User.h"

User::User(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password)
        : m_firstName(firstName), m_lastName(lastName), m_email(email), m_username(username), m_password(password)
{

}
const String &User::GetFirstName() const
{
    return m_firstName;
}
const String &User::GetLastName() const
{
    return m_lastName;
}
const String &User::GetUsername() const
{
    return m_username;
}
const String &User::GetEmail() const
{
    return m_email;
}
const String &User::GetPassword() const
{
    return m_password;
}
