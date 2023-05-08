#ifndef SUPERHEROES_SUPERHERO_H
#define SUPERHEROES_SUPERHERO_H

#include "../Utils/String.h"
#include "SuperheroElements.h"
#include "SuperheroMode.h"

class Superhero
{
    String m_firstName;
    String m_lastName;
    String m_nickname;
    SuperheroElements m_element;
    SuperheroMode m_mode;
    unsigned m_power;
    double m_cost;
    bool m_isDead;
public:
    Superhero();
    Superhero(const String &firstName, const String &lastName, const String &nickname, SuperheroElements element,
              unsigned power, double cost);

    void FightWith(const Superhero& hero);
};


#endif //SUPERHEROES_SUPERHERO_H
