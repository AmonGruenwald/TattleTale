#include "tale/kernels/interaction.hpp"

#include <iostream>

namespace tale
{
    Interaction::Interaction(std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(reasons){};
    std::string Interaction::ToString()
    {
        return "I am an Interaction.\n";
    }
} // namespace tale