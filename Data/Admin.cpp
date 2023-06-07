#include "Admin.h"

Admin::Admin(const String &firstName, const String &lastName, const String &email, const String &username,
             const String &password) : User(firstName, lastName, email, username, password)
{

}
UserType Admin::GetUserType() const
{
    return UserType::Admin;
}
