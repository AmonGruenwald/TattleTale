#include "tale/kernels/trait.hpp"

#include <iostream>

namespace tale
{
    Trait::Trait(std::string name, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons)
        : Kernel(name, id, tick, reasons){};
    std::string Trait::ToString()
    {
        return "Traits are not implemented yet.";
    }
} // namespace tale