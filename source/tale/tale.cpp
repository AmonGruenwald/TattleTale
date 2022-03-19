#include "shared/tattletalecore.hpp"
#include "tale/tale.hpp"

#include <iostream>
#include <memory>
#include <chrono>
#include <fmt/core.h>
namespace tattletale
{

    void Tale(Chronicle &chronicle, Random &random, const Setting &setting)
    {
        TATTLETALE_DEBUG_PRINT("TALE STARTED");
        TATTLETALE_DEBUG_PRINT("SETTING UP SCHOOL");
        School school(chronicle, random, setting);

        TATTLETALE_DEBUG_PRINT("STARTING SIMULATION");

        auto t1 = std::chrono::steady_clock::now();
        school.SimulateDays(setting.days_to_simulate);
        auto t2 = std::chrono::steady_clock::now();
        auto nano_seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        TATTLETALE_DEBUG_PRINT(fmt::format("SIMULATION TOOK {}ns.", nano_seconds));
    }
} // namespace tattletale