#include "tale/tale.hpp"

#include <iostream>

namespace tale
{

    void Run(const Setting &setting)
    {
        std::cout << "TALE STARTED" << std::endl;
        std::cout << "SETTING UP SCHOOL" << std::endl;
        School school(setting);

        size_t days_to_simulate = 30;
        school.SimulateDays(days_to_simulate);
    }
} // namespace tale