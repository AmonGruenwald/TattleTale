#include "tale/kernels/action.hpp"

#include <iostream>

namespace tale
{
    Action::Action(std::string name, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, reasons){};
    std::string Action::ToString()
    {
        return "I am an Action.\n";
    }
} // namespace tale