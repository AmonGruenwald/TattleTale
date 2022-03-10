#include "tale/kernels/resourcekernels/resource.hpp"

#include <iostream>
#include <fmt/core.h>
#include "tale/actor.hpp"

namespace tale
{
    Resource::Resource(std::string name, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
        : Kernel(name, id, tick, owner, reasons), value_(value){};
    float Resource::GetValue() const
    {
        return value_;
    }
    std::string Resource::ToString()
    {
        return fmt::format("{} has {} of {}", owner_.lock()->name_, name_, value_);
    }
} // namespace tale