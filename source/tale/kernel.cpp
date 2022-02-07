#include "tale/kernel.hpp"

#include <iostream>

namespace tale
{

    // TODO: Add day tracking once gameloop is implemented
    Kernel::Kernel(std::vector<std::weak_ptr<Kernel>> reasons)
    {
        reasons_ = reasons;
        number_ = current_number_;
        ++current_number_;
    }
    void Kernel::AddConsequence(std::weak_ptr<Kernel> consequence)
    {
        consequences_.push_back(consequence);
    }
} // namespace tale