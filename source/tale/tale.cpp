#include "tale/talecore.hpp"
#include "tale/tale.hpp"

#include <iostream>
namespace tale
{

    void Run(const Setting &setting)
    {
        TALE_DEBUG_PRINT("TALE STARTED\n");
        TALE_DEBUG_PRINT("SETTING UP SCHOOL\n");
        School school(setting);

        school.SimulateDays(setting.days_to_simulate);
    }
} // namespace tale