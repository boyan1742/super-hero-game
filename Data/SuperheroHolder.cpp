#include <iostream>
#include "SuperheroHolder.h"

SuperheroHolder SuperheroHolder::m_instance;

SuperheroHolder::SuperheroHolder()
        : m_holder(16), m_market(16)
{

}
const Superhero &SuperheroHolder::GetSuperhero(size_t uniqueID)
{
    for (int i = 0; i < m_holder.GetLength(); ++i)
    {
        if (m_holder[i].GetUniqueID() == uniqueID)
            return m_holder[i];
    }

    throw std::runtime_error((String("No superhero with id: ") + uniqueID + " wasn't found!").c_str());
}
void SuperheroHolder::AddSuperhero(Superhero &&superhero)
{
    m_holder.Add(std::move(superhero));
}
void SuperheroHolder::AddSuperhero(const Superhero &superhero)
{
    m_holder.Add(superhero);
}
SuperheroHolder &SuperheroHolder::GetInstance()
{
    return m_instance;
}
void SuperheroHolder::SaveMarket() const
{
    std::ofstream file("market.dat", std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "[ERROR] Cannot save market!\n";
        return;
    }

    size_t len = m_market.GetLength();
    file.write((const char *) &len, sizeof(size_t));
    for (int i = 0; i < len; ++i)
    {
        file.write((const char *) &m_market[i], sizeof(size_t));
    }

    file.close();
}
void SuperheroHolder::LoadMarket()
{
    std::ifstream file("market.dat", std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "[ERROR] Cannot load market!\n";
        return;
    }

    size_t len;
    file.read((char *) &len, sizeof(size_t));
    for (int i = 0; i < len; ++i)
    {
        file.read((char *) &m_market[i], sizeof(size_t));
    }

    file.close();
}
const Array<size_t> &SuperheroHolder::GetMarketHeroes() const
{
    return m_market;
}
bool SuperheroHolder::HasHeroes() const
{
    return !m_holder.IsEmpty();
}
