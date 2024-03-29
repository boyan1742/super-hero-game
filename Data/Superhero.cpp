#include "Superhero.h"

Superhero::Superhero(size_t uniqueID, const String &firstName, const String &lastName, const String &nickname, SuperheroElements element,
                     unsigned int power, double cost)
        : m_firstName(firstName), m_lastName(lastName), m_nickname(nickname)
{
    m_uniqueID = uniqueID;
    m_element = element;
    m_power = power;
    m_cost = cost;
    m_isDead = false;
    m_mode = SuperheroMode::Defence;
}
Superhero::Superhero()
        : m_firstName(), m_lastName(), m_nickname()
{
    m_uniqueID = 0;
    m_element = SuperheroElements::Fire;
    m_power = 0;
    m_cost = 0;
    m_isDead = true;
    m_mode = SuperheroMode::Defence;
}
void Superhero::FightWith(const Superhero &hero)
{

}
void Superhero::SetMode(SuperheroMode mode)
{
    m_mode = mode;
}
void Superhero::SetDead(bool dead)
{
    m_isDead = dead;
}
size_t Superhero::GetUniqueID() const
{
    return m_uniqueID;
}
const String &Superhero::GetNickname() const
{
    return m_nickname;
}
double Superhero::GetCost() const
{
    return m_cost;
}
SuperheroElements Superhero::GetElement() const
{
    return m_element;
}
const String &Superhero::GetFirstName() const
{
    return m_firstName;
}
const String &Superhero::GetLastName() const
{
    return m_lastName;
}
unsigned Superhero::GetPower() const
{
    return m_power;
}
SuperheroMode Superhero::GetMode() const
{
    return m_mode;
}
