#include "SHGSystem.h"
#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"

#include <iostream>


void SHGSystem::Run()
{
    bool quit = false;
    while (!quit)
    {
        String line;
        String::GetLine(std::cin, &line);
        Array<String> chunks = line.Split(' ');

        SHGSystemCommandType cmdType;
        try
        {
            cmdType = SHGSystemCommandStem::GetCommandType(chunks[0]);
        }
        catch (InvalidCommandException &ex)
        {
            std::cout << ex.what() << std::endl;
            continue;
        }
        catch (...)
        {
            std::cout << "Unknown error occurred!\nPress any key to exit!";
            std::cin.get();
            break;
        }

        switch (cmdType)
        {
            case SHGSystemCommandType::Quit:
                quit = true;
                break;
            case SHGSystemCommandType::Help:
                PrintHelpMessagePlayer();
                break;
            case SHGSystemCommandType::Login:
                break;
            case SHGSystemCommandType::Logout:
                break;
            case SHGSystemCommandType::AddPlayer:
                break;
            case SHGSystemCommandType::DeletePlayer:
                break;
            case SHGSystemCommandType::Market:
                break;
            case SHGSystemCommandType::Players:
                break;
            case SHGSystemCommandType::Buy:
                break;
            case SHGSystemCommandType::Stance:
                break;
            case SHGSystemCommandType::Attack:
                break;
            case SHGSystemCommandType::Baltop:
                break;
            case SHGSystemCommandType::Upgrade:
                break;
        }
    }
}
void SHGSystem::PrintHelpMessagePlayer()
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n" <<
              "\tMarket - shows which heroes are for sale in the market.\n" <<
              "\tPlayers - shows all players' statistics.\n" <<
              "\tBuy - buys a hero from the market.\n" <<
              "\tAttack - attacks a player.\n" <<
              "\tBaltop - shows top 3 players with most money.\n" <<
              "\tUpgrade - upgrades a hero using player's money.\n";
}
void SHGSystem::PrintHelpMessage()
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n";
}
void SHGSystem::PrintHelpMessageAdmin()
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n" <<
              "\tAddPlayer - adds new player.\n" <<
              "\tDeletePlayer - deletes a player.\n" <<
              "\tMarket - shows which heroes are for sale in the market.\n" <<
              "\tPlayers - shows all players' statistics.\n";
}
