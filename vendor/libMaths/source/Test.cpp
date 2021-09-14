#include "Test.h"

#include <iostream>

namespace LibMath
{
    void Test::HelloWorld(std::string indentation)
    {
        std::cout << indentation << "LibMath say : [Hello] to [World] !" << std::endl;
    }
}