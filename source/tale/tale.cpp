#include "shared/tattletalecore.hpp"
#include "tale/tale.hpp"

#include <iostream>
namespace tattletale
{

    Chronicle &Run(const Setting &setting)
    {
        TATTLETALE_DEBUG_PRINT("TALE STARTED");
        TATTLETALE_DEBUG_PRINT("SETTING UP SCHOOL");
        School school(setting);

        TATTLETALE_DEBUG_PRINT("STARTING SIMULATION");
        school.SimulateDays(setting.days_to_simulate);
        return school.GetChronicle();
    }
} // namespace tattletale