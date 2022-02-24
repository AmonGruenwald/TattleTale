#include "tale/talecore.hpp"
#include "tale/tale.hpp"

#include <iostream>
namespace tale
{

    void Run(const Setting &setting)
    {
        TALE_DEBUG_PRINT("TALE STARTED");
        TALE_DEBUG_PRINT("SETTING UP SCHOOL");
        School school(setting);

        TALE_DEBUG_PRINT("STARTING SIMULATION");
        school.SimulateDays(setting.days_to_simulate);
    }
} // namespace tale