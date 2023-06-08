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
    SuperheroHolder(const SuperheroHolder&) = delete;
    SuperheroHolder& operator=(const SuperheroHolder&) = delete;

    void AddSuperhero(const Superhero& superhero);
    void AddSuperhero(Superhero&& superhero);
    const Superhero& GetSuperhero(size_t uniqueID);
    bool HasHeroes() const;

    bool IsUniqueIDFree(size_t id) const;

    void SaveSuperheroes() const;
    void LoadSuperheroes();

    static SuperheroHolder& GetInstance();

    void SaveMarket() const;
    void LoadMarket();

    void AddHeroIDToMarket(size_t id);
    void RemoveHeroIDFromMarket(size_t id);

    const Array<size_t>& GetMarketHeroes() const;
};


#endif //SUPERHEROES_SUPERHEROHOLDER_H
