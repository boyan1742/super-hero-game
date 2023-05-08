#ifndef SUPERHEROES_PLAYER_H
#define SUPERHEROES_PLAYER_H

#include "Person.h"
#include "../Utils/Array.h"
#include "Superhero.h"

class Player : public Person
{
    Array<Superhero> m_heroes;
    double m_money;
public:
    Player(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password, double money);

    void AddHero(const Superhero& hero);
};


#endif //SUPERHEROES_PLAYER_H
