cmake_minimum_required(VERSION 3.25)
project(superHeroes)

set(CMAKE_CXX_STANDARD 17)

add_executable(superHeroes main.cpp Person.cpp Person.h Utils/String.cpp Utils/String.h Player.cpp Player.h Admin.cpp Admin.h Superhero.cpp Superhero.h SuperheroElements.h)
