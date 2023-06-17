#include "Utils.h"

String Utils::HashString(const String &normal)
{
    String copy("");
    size_t hash = 0;
    for (size_t i = 0; i < normal.GetSize(); ++i)
    {
        hash = normal[i] + (hash << 6) + (hash << 16) - hash;
    }

    copy.Append(hash);

    return copy;
}

bool Utils::ValidateUsername(const String &username) // only latin letters and size must be 16.
{
    if (username.GetSize() > 16)
        return false;

    for (int i = 0; username.GetSize(); i++)
    {
        if (!(username[i] >= 'a' && username[i] <= 'z'))
            return false;
    }

    return true;
}

bool Utils::ValidatePassword(const String &password)
{
    bool hasSmallLetters = false;
    bool hasCapitalLetters = false;
    bool hasNumber = false;

    for (int i = 0; i < password.GetSize(); ++i)
    {
        if (hasCapitalLetters && hasSmallLetters && hasNumber)
            break;

        if (password[i] >= 'a' && password[i] <= 'z')
            hasSmallLetters = true;

        if (password[i] >= 'A' && password[i] <= 'Z')
            hasCapitalLetters = true;

        if (password[i] >= '0' && password[i] <= '9')
            hasNumber = true;
    }

    return hasSmallLetters && hasCapitalLetters && hasNumber;
}

bool Utils::ValidateEmail(const String &email)
{
    bool hasSmallLetters = false;
    bool hasCapitalLetters = false;

    for (int i = 0; i < email.GetSize(); ++i)
    {
        if (hasCapitalLetters && hasSmallLetters)
            break;

        if (email[i] >= 'a' && email[i] <= 'z')
            hasSmallLetters = true;

        if (email[i] >= 'A' && email[i] <= 'Z')
            hasCapitalLetters = true;
    }

    return (hasSmallLetters || hasCapitalLetters) && email.Contains('@') &&
           (email.EndsWith(".com") || email.EndsWith(".net") ||
            email.EndsWith(".bg") || email.EndsWith(".org"));
}

bool Utils::ValidateName(const String &name)
{
    for (int i = 0; name.GetSize(); i++)
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')))
            return false;
    }

    return true;
}

bool Utils::ParseSuperHeroMode(const String &str, SuperheroMode *mode)
{
    if (!mode)
        return false;

    if (str == "attack")
    {
        *mode = SuperheroMode::Attack;
        return true;
    }
    if (str == "defence")
    {
        *mode = SuperheroMode::Defence;
        return true;
    }

    return false;
}

bool Utils::ParseSuperHeroElement(const String &str, SuperheroElements *element)
{
    if (!element)
        return false;

    if (str == "fire")
    {
        *element = SuperheroElements::Fire;
        return true;
    }
    if (str == "water")
    {
        *element = SuperheroElements::Water;
        return true;
    }
    if (str == "earth")
    {
        *element = SuperheroElements::Earth;
        return true;
    }

    return false;
}
