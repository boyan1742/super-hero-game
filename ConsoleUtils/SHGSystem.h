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
    size_t m_playerCount = 0;
    Array<String> m_loggedPlayers;

    void PrintWelcomeMessage() const;

    size_t GetPlayersCount() const;

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

    void AddPlayer() const;
    void DeletePlayer(String username) const;

    void SavePlayer(const Player& pl) const;

    void Market() const;
    void ShowPlayers() const;
    void BuyHero() const;

    void ChangeStance() const;
    void AttackPlayer() const;

    void UpgradeHero() const;
    void Baltop() const;

    bool AddAccountToFile(UserType type, const String &usernameHash) const;

    bool HasUserInUsersFile(const String& hashedUsername) const;

    Optional<UniquePtr<User>> ParseUser(std::ifstream &file) const;
public:
    SHGSystem(const SHGSystem&) = delete;
    SHGSystem& operator=(const SHGSystem&) = delete;

    static SHGSystem &GetInstance();

    void Run();
};


#endif //SUPERHEROES_SHGSYSTEM_H
