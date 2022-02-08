#include "tale/kernels/kernel.hpp"

#include <iostream>

namespace tale
{
    size_t Kernel::current_number_ = 0;
    // TODO: Add day tracking once gameloop is implemented
    Kernel::Kernel(std::string name, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        name_ = name;
        reasons_ = reasons;
        number_ = current_number_;
        ++current_number_;
    }
    void Kernel::AddConsequence(std::weak_ptr<Kernel> consequence)
    {
        consequences_.push_back(consequence);
    }
} // namespace tale