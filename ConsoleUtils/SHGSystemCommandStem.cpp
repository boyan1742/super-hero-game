#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"

class __CmdStems
{
public:
    SHGSystemCommandType _key;
    String _cmd;
};

static Array<__CmdStems> m_stems(
        {
                __CmdStems{SHGSystemCommandType::Help, "Help"},
                __CmdStems{SHGSystemCommandType::Login, "Login"},
                __CmdStems{SHGSystemCommandType::Logout, "Logout"},
                __CmdStems{SHGSystemCommandType::AddPlayer, "AddPlayer"},
                __CmdStems{SHGSystemCommandType::DeletePlayer, "DeletePlayer"},
                __CmdStems{SHGSystemCommandType::Market, "Market"},
                __CmdStems{SHGSystemCommandType::Players, "Players"},
                __CmdStems{SHGSystemCommandType::Buy, "Buy"},
                __CmdStems{SHGSystemCommandType::Stance, "Stance"},
                __CmdStems{SHGSystemCommandType::Attack, "Attack"},
                __CmdStems{SHGSystemCommandType::Baltop, "Baltop"},
                __CmdStems{SHGSystemCommandType::Upgrade, "Upgrade"},
                __CmdStems{SHGSystemCommandType::Quit, "Quit"},
        }, true);

bool SHGSystemCommandStem::IsStemValid(const String &stem)
{
    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if(m_stems[i]._cmd == stem)
            return true;
    }

    return false;
}
SHGSystemCommandType SHGSystemCommandStem::GetCommandType(const String &stem)
{
    if(!IsStemValid(stem))
        throw InvalidCommandException();

    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if(m_stems[i]._cmd == stem)
            return m_stems[i]._key;
    }

    return SHGSystemCommandType::Quit; // something went wrong, shouldn't ever come here!!!
}
