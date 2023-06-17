#include "Player.h"

Player::Player(const String &firstName, const String &lastName, const String &email, const String &username,
               const String &password, double money) : User(firstName, lastName, email, username, password)
{
    m_money = money;
}

void Player::AddHero(size_t heroID, SuperheroMode mode)
{
    m_heroes.Add(Pair(heroID, mode));
}

UserType Player::GetUserType() const
{
    return UserType::Player;
}

void Player::AddHeroes(const Array<size_t> &heroesIDs)
{
    for (int i = 0; i < heroesIDs.GetLength(); ++i)
    {
        m_heroes.Add(Pair(heroesIDs[i], SuperheroMode::Attack));
    }
}

void Player::AddHeroes(const Array<Pair<size_t, SuperheroMode>> &heroesIDs)
{
    for (int i = 0; i < heroesIDs.GetLength(); ++i)
    {
        m_heroes.Add(heroesIDs[i]);
    }
}

double Player::GetMoney() const
{
    return m_money;
}

const Array<Pair<size_t, SuperheroMode>> &Player::GetSuperheroes() const
{
    return m_heroes;
}
void Player::ChangeHeroStance(size_t heroID, SuperheroMode stance)
{
    for (int i = 0; i < m_heroes.GetLength(); ++i)
    {
        if (m_heroes[i].GetFirst() == heroID)
            m_heroes[i].SetSecond(stance);
    }
}
bool Player::HasSuperhero(size_t id)
{
    for (int i = 0; i < m_heroes.GetLength(); ++i)
    {
        if (m_heroes[i].GetFirst() == id)
            return true;
    }
    return false;
}
void Player::DecreaseMoney(double amount)
{
    m_money -= amount;
    if(m_money < 0)
        m_money = 0;
}
void Player::AddMoney(double amount)
{
    m_money += amount;
}
void Player::DeleteHero(size_t id)
{
    Array<Pair<size_t, SuperheroMode>> temp;
    for (int i = 0; i < m_heroes.GetLength(); ++i)
    {
        if(m_heroes[i].GetFirst() != id)
            temp.Add(std::move(m_heroes[i]));
    }

    m_heroes = std::move(temp);
}
