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

    size_t m_uniqueID;
public:
    Superhero();
    Superhero(size_t uniqueID, const String &firstName, const String &lastName, const String &nickname, SuperheroElements element,
              unsigned power, double cost);

    void FightWith(const Superhero& hero);
    void SetMode(SuperheroMode mode);
    void SetDead(bool dead);

    size_t GetUniqueID() const;

    const String& GetFirstName() const;
    const String& GetLastName() const;
    const String& GetNickname() const;
    unsigned GetPower() const;
    double GetCost() const;
    SuperheroMode GetMode() const;
    SuperheroElements GetElement() const;
};


#endif //SUPERHEROES_SUPERHERO_H
