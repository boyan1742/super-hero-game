#ifndef SUPERHEROES_SUPERHERO_H
#define SUPERHEROES_SUPERHERO_H

#include "Utils/String.h"
#include "SuperheroElements.h"

class Superhero
{
    String m_firstName;
    String m_lastName;
    String m_nickname;
    SuperheroElements m_element;
    unsigned m_power;
    double m_cost;
public:
    Superhero(const String &firstName, const String &lastName, const String &nickname, SuperheroElements element,
              unsigned power, double cost);
};


#endif //SUPERHEROES_SUPERHERO_H
