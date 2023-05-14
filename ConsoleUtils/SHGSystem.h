#ifndef SUPERHEROES_SHGSYSTEM_H
#define SUPERHEROES_SHGSYSTEM_H


#include "../Utils/String.h"
#include "../Data/Person.h"

class SHGSystem
{
    static SHGSystem m_instance;

    SHGSystem() = default;

    Person* m_loggedUser = nullptr;

    void PrintHelpMessagePlayer();
    void PrintHelpMessageAdmin();
    void PrintHelpMessage();
public:
    static const SHGSystem& GetInstance();

    void Run();
};


#endif //SUPERHEROES_SHGSYSTEM_H
