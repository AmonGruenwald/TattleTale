#include "tale/talecore.hpp"
#include "tale/kernels/kernel.hpp"

#include <iostream>

namespace tale
{
    Kernel::Kernel(
        std::string name, size_t id,
        size_t tick,
        std::vector<std::weak_ptr<Kernel>> reasons)
        : name_(name),
          id_(id),
          tick_(tick),
          reasons_(reasons) {}
    void Kernel::AddConsequence(std::weak_ptr<Kernel> consequence)
    {
        consequences_.push_back(consequence);
    }

} // namespace tale