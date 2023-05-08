cmake_minimum_required(VERSION 3.25)
project(superHeroes)

set(CMAKE_CXX_STANDARD 17)

add_executable(superHeroes main.cpp Data/Person.cpp Data/Person.h Utils/String.cpp Utils/String.h Data/Player.cpp Data/Player.h Data/Admin.cpp Data/Admin.h Data/Superhero.cpp Data/Superhero.h Data/SuperheroElements.h Utils/Array.cpp Utils/Array.h Exceptions/IndexOutOfRangeException.cpp Exceptions/IndexOutOfRangeException.h Data/SuperheroMode.h ConsoleUtils/SHGSystem.cpp ConsoleUtils/SHGSystem.h ConsoleUtils/SHGSystemCommandStem.cpp ConsoleUtils/SHGSystemCommandStem.h Exceptions/InvalidCommandException.cpp Exceptions/InvalidCommandException.h)
