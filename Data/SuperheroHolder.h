#ifndef SUPERHEROES_SUPERHEROHOLDER_H
#define SUPERHEROES_SUPERHEROHOLDER_H


#include "../Utils/Array.hpp"
#include "Superhero.h"

class SuperheroHolder
{
    mutable Array<Superhero> m_holder;
    mutable Array<size_t> m_market;

    static SuperheroHolder m_instance;
    SuperheroHolder();
public:
    void AddSuperhero(const Superhero& superhero) const;
    void AddSuperhero(Superhero&& superhero) const;
    const Superhero& GetSuperhero(size_t uniqueID) const;
    bool HasHeroes() const;

    static const SuperheroHolder& GetInstance();

    void SaveMarket() const;
    void LoadMarket() const;

    const Array<size_t>& GetMarketHeroes() const;
};


#endif //SUPERHEROES_SUPERHEROHOLDER_H
