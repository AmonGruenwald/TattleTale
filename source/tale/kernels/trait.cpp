#include "tale/kernels/trait.hpp"

#include <iostream>

namespace tale
{
    Trait::Trait(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, tick, reasons){};
    std::string Trait::ToString()
    {
        return "I am a Trait.\n";
    }
} // namespace tale