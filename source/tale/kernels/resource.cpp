#include "tale/kernels/resource.hpp"

#include <iostream>

namespace tale
{
    Resource::Resource(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, tick, reasons){};
    std::string Resource::ToString()
    {
        return "I am a Resource.\n";
    }
} // namespace tale