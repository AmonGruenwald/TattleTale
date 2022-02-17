#include "tale/kernels/goal.hpp"

#include <iostream>

namespace tale
{
    Goal::Goal(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, tick, reasons){};
    std::string Goal::ToString()
    {
        return "Goals are not implented yet.";
    }
} // namespace tale