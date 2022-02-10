#include "tale/tale.hpp"

#include <iostream>

namespace tale
{

    void Run()
    {
        std::cout << "TALE STARTED" << std::endl;
        std::cout << "INITIALIZING INTERACTION STORE" << std::endl;
        InteractionStore interaction_store;
        std::cout << "SETTING UP SCHOOL" << std::endl;
        School school(300);
    }
} // namespace tale