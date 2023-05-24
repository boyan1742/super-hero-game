#include "Player.h"

Player::Player(const String &firstName, const String &lastName, const String &email, const String &username,
               const String &password, double money) : User(firstName, lastName, email, username, password)
{
    m_money = money;
}

void Player::AddHero(size_t heroID)
{
    m_heroes.Add(heroID);
}
UserType Player::GetUserType()
{
    return UserType::Player;
}
void Player::AddHeroes(const Array<size_t> &heroesIDs)
{
    for (int i = 0; i < heroesIDs.GetLength(); ++i)
    {
        m_heroes.Add(heroesIDs[i]);
    }
}
