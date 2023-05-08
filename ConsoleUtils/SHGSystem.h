#ifndef SUPERHEROES_SHGSYSTEM_H
#define SUPERHEROES_SHGSYSTEM_H


#include "../Utils/String.h"

class SHGSystem
{
    static SHGSystem m_instance;

    SHGSystem() = default;
public:
    static const SHGSystem& GetInstance();

    bool ParseCommand(const String& cmd);

    void Run();
};


#endif //SUPERHEROES_SHGSYSTEM_H
