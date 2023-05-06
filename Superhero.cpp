//
// Created by boyan on 3.5.2023 г..
//

#include "Superhero.h"

Superhero::Superhero(const String &firstName, const String &lastName, const String &nickname, SuperheroElements element,
                     unsigned int power, double cost)
        : m_firstName(firstName), m_lastName(lastName), m_nickname(nickname)
{
    m_element = element;
    m_power = power;
    m_cost = cost;
}
