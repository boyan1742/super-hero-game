#include <iostream>
#include "Logger.h"

void Logger::LogMessage(const String &str)
{
    std::cout << "\n[MESSAGE] " << str << "\n";
}
void Logger::LogWarning(const String &str)
{
    std::cout << "\n[WARNING] " << str << "\n";
}
void Logger::LogError(const String &str)
{
    std::cout << "\n[ERROR] " << str << "\n";
}
void Logger::LogFatal(const String &str, int exitCode)
{
    std::cout << "\n[FATAL ERROR] " << str << "\n";
    std::cout << "\nPress enter to exit.\n";
    std::cin.get();
    exit(exitCode);
}

void Logger::LogFatal(const std::exception &e, int exitCode)
{
    std::cout << "\n[FATAL ERROR] " << e.what() << "\n";
    std::cout << "\nPress enter to exit.\n";
    std::cin.get();
    exit(exitCode);
}
