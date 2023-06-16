#include <iostream>
#include "SuperheroHolder.h"
#include "../Utils/Logger.h"

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
        file.write((const char *) &m_market[i], sizeof(size_t));

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

    m_market = Array<size_t>();

    size_t len;
    file.read((char *) &len, sizeof(size_t));
    for (int i = 0; i < len; ++i)
    {
        size_t id = 0;
        file.read((char *) &id, sizeof(size_t));

        m_market.Add(id);
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
void SuperheroHolder::SaveSuperheroes() const
{
    std::ofstream file("sh.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Cannot save superheroes to file!");
        return;
    }

    size_t len = m_holder.GetLength();
    file.write((const char *) &len, sizeof(size_t));

    for (int i = 0; i < len; ++i)
    {
        //layout:
        //          hUniqueID|hfnSize|hFirstName|hlnSize|hLastName|hnSize|hNickname|hElement|hMode|hPower|hCost

        SuperheroElements hElement = m_holder[i].GetElement();
        //SuperheroMode hMode = m_holder[i].GetMode();
        unsigned hPower = m_holder[i].GetPower();
        double hCost = m_holder[i].GetCost();

        size_t hfnSize = m_holder[i].GetFirstName().GetSize() + 1, hlnSize = m_holder[i].GetLastName().GetSize() + 1,
                hnSize = m_holder[i].GetNickname().GetSize() + 1, hUniqueID = m_holder[i].GetUniqueID();

        file.write((const char *) &hUniqueID, sizeof(size_t));

        file.write((const char *) &hfnSize, sizeof(size_t));
        file.write(m_holder[i].GetFirstName().c_str(), hfnSize);

        file.write((const char *) &hlnSize, sizeof(size_t));
        file.write(m_holder[i].GetLastName().c_str(), hlnSize);

        file.write((const char *) &hnSize, sizeof(size_t));
        file.write(m_holder[i].GetNickname().c_str(), hnSize);

        file.write((const char *) &hElement, sizeof(SuperheroElements));
        //file.write((const char *) &hMode, sizeof(SuperheroMode));
        file.write((const char *) &hPower, sizeof(unsigned));
        file.write((const char *) &hCost, sizeof(double));
    }

}
void SuperheroHolder::LoadSuperheroes()
{
    std::ifstream file("sh.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogWarning("No heroes are present! Contact an admin.");
        return;
    }

    size_t len;
    file.read((char *) &len, sizeof(size_t));

    for (int i = 0; i < len; ++i)
    {
        //layout:
        //          hUniqueID|hfnSize|hFirstName|hlnSize|hLastName|hnSize|hNickname|hElement|hMode|hPower|hCost

        String hFirstName, hLastName, hNickname;
        SuperheroElements hElement;
        SuperheroMode hMode;
        unsigned hPower;
        double hCost;

        size_t hfnSize, hlnSize, hnSize, hUniqueID;

        file.read((char *) &hUniqueID, sizeof(size_t));

        file.read((char *) &hfnSize, sizeof(size_t));
        String::ReadExact(file, hfnSize, &hFirstName);
        file.read((char *) &hlnSize, sizeof(size_t));
        String::ReadExact(file, hlnSize, &hLastName);
        file.read((char *) &hnSize, sizeof(size_t));
        String::ReadExact(file, hnSize, &hNickname);

        file.read((char *) &hElement, sizeof(SuperheroElements));
        //file.read((char *) &hMode, sizeof(SuperheroMode)); // if we save the mode, all players that have this SH will share the same mode! TODO: FIX!!!
        file.read((char *) &hPower, sizeof(unsigned));
        file.read((char *) &hCost, sizeof(double));

        AddSuperhero(Superhero(hUniqueID, hFirstName, hLastName,
                               hNickname, hElement, hPower, hCost));
    }

    file.close();
}
bool SuperheroHolder::IsUniqueIDFree(size_t id) const
{
    for (int i = 0; i < m_holder.GetLength(); ++i)
    {
        if (m_holder[i].GetUniqueID() == id)
            return false;
    }

    return true;
}
void SuperheroHolder::AddHeroIDToMarket(size_t id)
{
    if (IsUniqueIDFree(id) ||
        m_market.Contains(id)) // this id doesn't belong to a hero OR this hero is in the market already!
        return;

    m_market.Add(id);
    SaveMarket();
}
void SuperheroHolder::RemoveHeroIDFromMarket(size_t id)
{
    if (!m_market.Contains(id))
        return;

    Array<size_t> idsToKeep;
    for (int i = 0; i < m_market.GetLength(); ++i)
    {
        if (m_market[i] == id)
            continue;

        idsToKeep.Add(m_market[i]);
    }

    m_market = idsToKeep;
    SaveMarket();
}
const Superhero &SuperheroHolder::GetSuperhero(const String &nickname)
{
    for (int i = 0; i < m_holder.GetLength(); ++i)
    {
        if (m_holder[i].GetNickname() == nickname)
            return m_holder[i];
    }

    throw std::runtime_error(("No hero found with name: " + nickname + " !").c_str());
}
