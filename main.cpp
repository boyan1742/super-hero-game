#include <iostream>

#include "Utils/String.h"

int main()
{
    String s = "ABC xyz  o pi uuujj";

    auto val = s.Split(' ');

    return 0;
}
