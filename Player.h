#ifndef SUPERHEROES_PLAYER_H
#define SUPERHEROES_PLAYER_H

#include "Person.h"

class Player : public Person
{
    double m_money;
public:
    Player(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password, double money);
};


#endif //SUPERHEROES_PLAYER_H
