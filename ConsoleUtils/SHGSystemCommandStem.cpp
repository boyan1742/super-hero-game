#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"

class __CmdStems
{
public:
    SHGSystemCommandType _key;
    String _cmd;
};

static Array<__CmdStems> m_stems(
        new __CmdStems[]{
                __CmdStems{SHGSystemCommandType::Help, "help"},
                __CmdStems{SHGSystemCommandType::Login, "login"},
                __CmdStems{SHGSystemCommandType::Logout, "logout"},
                __CmdStems{SHGSystemCommandType::AddPlayer, "addplayer"},
                __CmdStems{SHGSystemCommandType::DeletePlayer, "deleteplayer"},
                __CmdStems{SHGSystemCommandType::Market, "market"},
                __CmdStems{SHGSystemCommandType::Players, "players"},
                __CmdStems{SHGSystemCommandType::Buy, "buy"},
                __CmdStems{SHGSystemCommandType::Stance, "stance"},
                __CmdStems{SHGSystemCommandType::Attack, "attack"},
                __CmdStems{SHGSystemCommandType::Baltop, "baltop"},
                __CmdStems{SHGSystemCommandType::Upgrade, "upgrade"},
                __CmdStems{SHGSystemCommandType::Quit, "quit"},
        }, 12, true);

bool SHGSystemCommandStem::IsStemValid(const String &stem)
{
    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if (m_stems[i]._cmd == stem)
            return true;
    }

    return false;
}
SHGSystemCommandType SHGSystemCommandStem::GetCommandType(const String &stem)
{
    String cpy = stem;
    cpy.ToLower();

    if (!IsStemValid(cpy))
        throw InvalidCommandException();

    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if (m_stems[i]._cmd == cpy)
            return m_stems[i]._key;
    }

    throw std::runtime_error("Something went wrong! This shouldn't be ever seen!");
}
