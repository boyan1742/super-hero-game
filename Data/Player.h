#ifndef SUPERHEROES_PLAYER_H
#define SUPERHEROES_PLAYER_H

#include "User.h"
#include "../Utils/Array.hpp"
#include "../Utils/Pair.hpp"
#include "Superhero.h"

class Player : public User
{
    Array<Pair<size_t, SuperheroMode>> m_heroes; //holds the IDs of the heroes and their stance for the current player.
    double m_money;
public:
    Player(const String &firstName, const String &lastName, const String &email, const String &username,
           const String &password, double money);

    void AddHero(size_t heroID, SuperheroMode mode = SuperheroMode::Attack);
    void AddHeroes(const Array<size_t>& heroesIDs);
    void AddHeroes(const Array<Pair<size_t, SuperheroMode>>& heroesIDs);
    UserType GetUserType() const override;
    double GetMoney() const;
    const Array<Pair<size_t, SuperheroMode>>& GetSuperheroes() const;

    void ChangeHeroStance(size_t heroID, SuperheroMode stance);
};


#endif //SUPERHEROES_PLAYER_H
