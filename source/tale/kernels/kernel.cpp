#include "tale/talecore.hpp"
#include "tale/kernels/kernel.hpp"

#include <iostream>

namespace tale
{
    size_t Kernel::current_number_ = 0;
    Kernel::Kernel(
        std::string name,
        size_t tick,
        std::vector<std::weak_ptr<Kernel>> reasons)
        : name_(name),
          tick_(tick),
          reasons_(reasons)
    {
        number_ = current_number_;
        ++current_number_;
    }
    Kernel::Kernel()
    {
        TALE_DEBUG_PRINT("Kernel default constructor\n");
    }
    void Kernel::AddConsequence(std::weak_ptr<Kernel> consequence)
    {
        consequences_.push_back(consequence);
    }

} // namespace tale