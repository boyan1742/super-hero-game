#ifndef SUPERHEROES_PLAYER_H
#define SUPERHEROES_PLAYER_H

#include "User.h"
#include "../Utils/Array.hpp"
#include "Superhero.h"

class Player : public User
{
    Array<size_t> m_heroes; //holds only the IDs of the heroes.
    double m_money;
public:
    Player(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password, double money);

    void AddHero(size_t heroID);
    void AddHeroes(const Array<size_t>& heroesIDs);
    UserType GetUserType() override;
    double GetMoney() const;
    const Array<size_t>& GetSuperheroes() const;
};


#endif //SUPERHEROES_PLAYER_H
