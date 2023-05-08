#include "SHGSystem.h"
#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"

#include <iostream>


void SHGSystem::Run()
{
    while(1)
    {
        String line;
        String::GetLine(std::cin, &line);

        SHGSystemCommandType cmdType;
        try
        {
            cmdType = SHGSystemCommandStem::GetCommandType(line);
        }
        catch(InvalidCommandException& ex)
        {
            std::cout << ex.what() << std::endl;
            continue;
        }
        catch(...)
        {
            std::cout << "Unknown error occurred!\nPress any key to exit!";
            std::cin.get();
            break;
        }
    }
}
