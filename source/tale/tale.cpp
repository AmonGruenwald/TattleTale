#include "shared/tattletalecore.hpp"
#include "tale/tale.hpp"

#include <iostream>
#include <memory>
namespace tattletale
{

    void Tale(Chronicle &chronicle, Random &random, const Setting &setting)
    {
        TATTLETALE_DEBUG_PRINT("TALE STARTED");
        TATTLETALE_DEBUG_PRINT("SETTING UP SCHOOL");
        School school(chronicle, random, setting);

        TATTLETALE_DEBUG_PRINT("STARTING SIMULATION");
        school.SimulateDays(setting.days_to_simulate);
    }
} // namespace tattletale