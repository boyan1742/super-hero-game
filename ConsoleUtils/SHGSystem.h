#ifndef SUPERHEROES_SHGSYSTEM_H
#define SUPERHEROES_SHGSYSTEM_H


#include "../Utils/String.h"
#include "../Data/User.h"

class SHGSystem
{
    static SHGSystem m_instance;

    SHGSystem() = default;

    User *m_loggedUser = nullptr;

    void Setup();
    void SetupHeroes() const;
    void CheckIfAdminAccountExists() const;

    void PrintHelpMessagePlayer() const;
    void PrintHelpMessageAdmin() const;
    void PrintHelpMessage() const;

    void Login();
    void Logout(bool bypass = false);
    bool CreateAdmin() const;
    void CraetePlayer() const;

    bool AddAccountToFile(UserType type, const String &usernameHash) const;
public:
    static SHGSystem &GetInstance();

    void Run();
};


#endif //SUPERHEROES_SHGSYSTEM_H
