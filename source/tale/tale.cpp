#include "tale/tale.hpp"

#include <iostream>

namespace tale
{

    void Run()
    {
        std::cout << "TALE STARTED" << std::endl;
        std::cout << "INITIALIZING UP RNG" << std::endl;
        std::cout << "INITIALIZING INTERACTION STORE" << std::endl;
        Random random;
        InteractionStore interaction_store;
        std::cout << "SETTING UP SCHOOL" << std::endl;
        School school(random, 300);

        size_t days_to_simulate = 30;
        school.SimulateDays(days_to_simulate);
    }
} // namespace tale