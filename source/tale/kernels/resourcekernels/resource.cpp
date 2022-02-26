#include "tale/kernels/resourcekernels/resource.hpp"

#include <iostream>
#include <fmt/core.h>

namespace tale
{
    Resource::Resource(std::string name, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value)
        : Kernel(name, id, tick, reasons), value_(value){};
    float Resource::GetValue() const
    {
        return value_;
    }
    std::string Resource::ToString()
    {
        return fmt::format("{}: {}", name_, value_);
    }
} // namespace tale