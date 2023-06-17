#ifndef SUPERHEROES_SHGSYSTEMCOMMANDSTEM_H
#define SUPERHEROES_SHGSYSTEMCOMMANDSTEM_H

#include "../Utils/String.h"

enum class SHGSystemCommandType
{
    Help,
    Login,
    Logout,
    AddPlayer,
    DeletePlayer,
    AddAdmin,
    TerminateProfile,
    Market,
    Players,
    Buy,
    Stance,
    Attack,
    Baltop,
    Upgrade,
    Quit
};

/*
 * The beginning of a command.
 * Examples: login <params>, quit, attack <params>, ...
*/
class SHGSystemCommandStem
{
public:
    SHGSystemCommandStem() = delete;

    static SHGSystemCommandType GetCommandType(const String& stem);
    static bool IsStemValid(const String& stem);
};


#endif //SUPERHEROES_SHGSYSTEMCOMMANDSTEM_H
