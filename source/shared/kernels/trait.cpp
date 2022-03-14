#include "shared/kernels/trait.hpp"

#include <iostream>

namespace tattletale
{
    Trait::Trait(std::string name, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons)
        : Kernel(name, id, tick, owner, reasons, KernelType::kTrait){};
    std::string Trait::ToString()
    {
        return "Traits are not implemented yet.";
    }
} // namespace tattletale