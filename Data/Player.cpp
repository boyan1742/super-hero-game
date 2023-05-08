#include "Player.h"

Player::Player(const String &firstName, const String &lastName, const String &email, const String &username,
               const String &password, double money) : Person(firstName, lastName, email, username, password)
{
    m_money = money;
}

void Player::AddHero(const Superhero &hero)
{
    m_heroes.Add(hero);
}
