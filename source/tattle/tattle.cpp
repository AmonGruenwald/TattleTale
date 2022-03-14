#include "tattle/tattle.hpp"

#include <iostream>
#include "shared/tattletalecore.hpp"
#include "tattle/curator.hpp"
#include <fmt/core.h>

namespace tattletale
{
    std::string Tattle(const Chronicle &chronicle, const Setting &setting)
    {
        TATTLETALE_DEBUG_PRINT("TATTLE STARTED");
        Curator curator(chronicle, setting);
        return curator.Curate();
    }
} // namespace tattletale