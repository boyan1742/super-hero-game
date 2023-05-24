#ifndef SUPERHEROES_SUPERHEROHOLDER_H
#define SUPERHEROES_SUPERHEROHOLDER_H


#include "../Utils/Array.hpp"
#include "Superhero.h"

class SuperheroHolder
{
    mutable Array<Superhero> m_holder;

    static SuperheroHolder m_instance;
    SuperheroHolder();
public:
    void AddSuperhero(const Superhero& superhero) const;
    void AddSuperhero(Superhero&& superhero) const;
    const Superhero& GetSuperhero(size_t uniqueID) const;

    static const SuperheroHolder& GetInstance();
};


#endif //SUPERHEROES_SUPERHEROHOLDER_H
