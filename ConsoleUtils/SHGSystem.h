#ifndef SUPERHEROES_SHGSYSTEM_H
#define SUPERHEROES_SHGSYSTEM_H


#include "../Utils/String.h"
#include "../Utils/Optional.hpp"
#include "../Utils/UniquePtr.hpp"
#include "../Data/User.h"
#include "../Data/Superhero.h"
#include "../Data/Player.h"

class SHGSystem
{
    static SHGSystem m_instance;

    SHGSystem() = default;

    User *m_loggedUser = nullptr;

    void PrintWelcomeMessage() const;

    void Setup();
    void CheckIfAdminAccountExists() const;
    void AddFirstTimeHeroes() const;

    void PrintHelpMessagePlayer() const;
    void PrintHelpMessageAdmin() const;
    void PrintHelpMessage() const;

    void Login();
    void Logout(bool bypass = false);
    bool CreateAdmin() const;
    bool CreatePlayer() const;

    Optional<Superhero> CreateHero() const;

    void AddPlayer();
    void DeletePlayer();

    void SavePlayer(const Player& pl);

    void Market();
    void ShowPlayers();
    void BuyHero();

    void ChangeStance();
    void AttackPlayer();

    void UpgradeHero();
    void Baltop();

    bool AddAccountToFile(UserType type, const String &usernameHash) const;

    bool HasUserInUsersFile(const String& hashedUsername) const;

    Optional<UniquePtr<User>> ParseUser(std::ifstream &file);
public:
    SHGSystem(const SHGSystem&) = delete;
    SHGSystem& operator=(const SHGSystem&) = delete;

    static SHGSystem &GetInstance();

    void Run();
};


#endif //SUPERHEROES_SHGSYSTEM_H
