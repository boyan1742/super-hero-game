#ifndef SUPERHEROES_SUPERHEROHOLDER_H
#define SUPERHEROES_SUPERHEROHOLDER_H

#include "../Utils/Array.hpp"
#include "Superhero.h"

class SuperheroHolder
{
    Array<Superhero> m_holder;
    Array<size_t> m_market;

    static SuperheroHolder m_instance;
    SuperheroHolder();
public:
    void AddSuperhero(const Superhero& superhero);
    void AddSuperhero(Superhero&& superhero);
    const Superhero& GetSuperhero(size_t uniqueID);
    bool HasHeroes() const;

    static SuperheroHolder& GetInstance();

    void SaveMarket() const;
    void LoadMarket();

    const Array<size_t>& GetMarketHeroes() const;
};


#endif //SUPERHEROES_SUPERHEROHOLDER_H
