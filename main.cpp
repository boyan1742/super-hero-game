#include <iostream>
#include "ConsoleUtils/SHGSystem.h"
#include "Utils/Logger.h"

int main()
{
    try
    {
        SHGSystem::GetInstance().Run();
    }
    catch (std::exception &e)
    {
        Logger::LogFatal(String("Uncaught exception: \n") + e.what());
    }
    catch (...)
    {
        Logger::LogFatal("Something else then an exception was thrown!", -2);
    }

    return 0;
}
