#ifndef SUPERHEROES_LOGGER_H
#define SUPERHEROES_LOGGER_H


#include "String.h"

class Logger
{
private:
    Logger() = default;
public:
    static void LogMessage(const String& str);
    static void LogWarning(const String& str);
    static void LogError(const String& str);
    static void LogFatal(const String& str, int exitCode = -1);
    static void LogFatal(const std::exception& e, int exitCode = -1);
};


#endif
