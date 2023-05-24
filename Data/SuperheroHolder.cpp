#include "SuperheroHolder.h"

SuperheroHolder SuperheroHolder::m_instance;

SuperheroHolder::SuperheroHolder()
        : m_holder(16)
{

}
const Superhero &SuperheroHolder::GetSuperhero(size_t uniqueID) const
{
    for (int i = 0; i < m_holder.GetLength(); ++i)
    {
        if (m_holder[i].GetUniqueID() == uniqueID)
            return m_holder[i];
    }

    throw std::runtime_error((String("No superhero with id: ") + uniqueID + " wasn't found!").c_str());
}
void SuperheroHolder::AddSuperhero(Superhero &&superhero) const
{
    m_holder.Add(std::move(superhero));
}
void SuperheroHolder::AddSuperhero(const Superhero &superhero) const
{
    m_holder.Add(superhero);
}
const SuperheroHolder &SuperheroHolder::GetInstance()
{
    return m_instance;
}
