#include "shared/tattletalecore.hpp"
#include "shared/kernels/kernel.hpp"

#include <iostream>

namespace tattletale
{
    Kernel::Kernel(
        std::string name, size_t id,
        size_t tick,
        std::weak_ptr<Actor> owner,
        std::vector<std::weak_ptr<Kernel>> reasons,
        KernelType type)
        : name_(name),
          id_(id),
          tick_(tick),
          owner_(owner),
          reasons_(reasons),
          type_(type)
    {
    }
    void Kernel::AddConsequence(std::weak_ptr<Kernel> consequence)
    {
        consequences_.push_back(consequence);
    }
    const std::vector<std::weak_ptr<Kernel>> &Kernel::GetConsequences() const
    {
        return consequences_;
    }

    float Kernel::GetChance() const { return 1.0f; }
    std::string Kernel::GetActiveDescription() { return ToString(); }
    const std::vector<std::weak_ptr<Kernel>> &Kernel::GetReasons() const
    {
        return reasons_;
    }
    std::weak_ptr<Actor> Kernel::GetOwner() const
    {
        return owner_;
    }
} // namespace tattletale