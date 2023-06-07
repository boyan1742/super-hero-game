#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"
#include "../Utils/Pair.hpp"

static bool m_initializedStems = false;
static Array<Pair<SHGSystemCommandType, String>> m_stems(13, true);

static void InitializeStems()
{
    if(m_initializedStems)
        return;

    const int length = 13;
    Pair<SHGSystemCommandType, String> stems[length] = {
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Help, "help"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Login, "login"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Logout, "logout"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::AddPlayer, "addplayer"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::DeletePlayer, "deleteplayer"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Market, "market"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Players, "players"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Buy, "buy"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Stance, "stance"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Attack, "attack"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Baltop, "baltop"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Upgrade, "upgrade"),
            Pair<SHGSystemCommandType, String>(SHGSystemCommandType::Quit, "quit"),
    };

    for (int i = 0; i < length; ++i)
        m_stems.Add(std::move(stems[i]));

    m_initializedStems = true;
}

bool SHGSystemCommandStem::IsStemValid(const String &stem)
{
    if(!m_initializedStems) InitializeStems();

    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if (m_stems[i].GetSecond() == stem)
            return true;
    }

    return false;
}
SHGSystemCommandType SHGSystemCommandStem::GetCommandType(const String &stem)
{
    if(!m_initializedStems) InitializeStems();

    String cpy = stem;
    cpy.ToLower();

    if (!IsStemValid(cpy))
        throw InvalidCommandException();

    for (int i = 0; i < m_stems.GetLength(); ++i)
    {
        if (m_stems[i].GetSecond() == cpy)
            return m_stems[i].GetFirst();
    }

    throw std::runtime_error("Something went wrong! This shouldn't be ever seen!");
}
