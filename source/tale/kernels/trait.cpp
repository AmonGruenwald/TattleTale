#include "tale/kernels/trait.hpp"

#include <iostream>

namespace tale
{
    Trait::Trait(std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(reasons){};
    std::string Trait::ToString()
    {
        return "I am a Trait.\n";
    }
} // namespace tale