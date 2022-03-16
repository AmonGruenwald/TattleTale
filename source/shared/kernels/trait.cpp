#include "shared/kernels/trait.hpp"

#include <iostream>

namespace tattletale
{
    Trait::Trait(std::string name, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons)
        : Kernel(name, id, tick, owner, reasons, KernelType::kTrait){};

    std::string Trait::GetDefaultDescription() const
    {
        return "WIP Traits";
    }
    std::string Trait::GetPassiveDescription() const
    {
        return GetDefaultDescription();
    }
    std::string Trait::GetActiveDescription() const
    {
        return GetDefaultDescription();
    }
} // namespace tattletale