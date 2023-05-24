#include "Utils.h"

String Utils::HashString(const String &normal)
{
    String copy;
    size_t hash = 0;
    for (size_t i = 0; i < normal.GetSize(); ++i)
    {
        hash = normal[i] + (hash << 6) + (hash << 16) - hash;
    }

    copy.Append(hash);

    return copy;
}
bool Utils::ValidateUsername(const String &username)
{
    bool flagA = false;
    bool flagN = false;

    for (char i = 'a'; i < 'z'; ++i)
    {
        flagA = username.Contains(i);
        if (flagA) break;

        flagA = username.Contains(i ^ 0x20);
        if (flagA) break;
    }

    for (char i = '0'; i < '9'; ++i)
    {
        flagN = username.Contains(i);
        if (flagN) break;
    }

    return flagA || flagN || username.Contains('-') || username.Contains('_');
}
bool Utils::ValidatePassword(const String &password)
{
    bool flagA = false;
    bool flagN = false;

    for (char i = 'a'; i < 'z'; ++i)
    {
        flagA = password.Contains(i);
        if (flagA) break;

        flagA = password.Contains(i ^ 0x20);
        if (flagA) break;
    }

    for (char i = '0'; i < '9'; ++i)
    {
        flagN = password.Contains(i);
        if (flagN) break;
    }

    return password.GetSize() >= 6 && flagA && flagN ||
           (password.Contains('!') || password.Contains('@') ||
            password.Contains('#') || password.Contains('$') ||
            password.Contains(' ') || password.Contains('-'));
}
bool Utils::ValidateEmail(const String &email)
{
    bool flagA = false;
    bool flagN = false;

    for (char i = 'a'; i < 'z'; ++i)
    {
        flagA = email.Contains(i);
        if (flagA) break;

        flagA = email.Contains(i ^ 0x20);
        if (flagA) break;
    }

    for (char i = '0'; i < '9'; ++i)
    {
        flagN = email.Contains(i);
        if (flagN) break;
    }

    return (flagA && email.Contains('@') && email.Contains('.')) ||
           (flagA && email.Contains('@') && email.Contains('.') && email.Contains('-')) ||
           (flagA && email.Contains('@') && email.Contains('.') && email.Contains('-') && flagN);
}
bool Utils::ValidateName(const String &name)
{
    bool flagA;

    for (char i = 'a'; i < 'z'; ++i)
    {
        flagA = name.Contains(i);
        if (flagA) return true;

        flagA = name.Contains(i ^ 0x20);
        if (flagA) return true;
    }

    return false;
}
