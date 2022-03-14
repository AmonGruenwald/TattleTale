#include "shared/kernels/resourcekernels/resource.hpp"

#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tattletale
{
    Resource::Resource(std::string name, std::string positive_name_variant, std::string negative_name_variant, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value, KernelType type)
        : Kernel(name, id, tick, owner, reasons, type), positive_name_variant_(positive_name_variant), negative_name_variant_(negative_name_variant), value_(value){};
    float Resource::GetValue() const
    {
        return value_;
    }
    std::string Resource::GetNameVariant()
    {
        if (value_ < 0)
        {
            return negative_name_variant_;
        }
        return positive_name_variant_;
    }
    std::string Resource::ToString()
    {
        std::string description = "";
        if (abs(value_) < 0.3f)
        {
            description += adjectives_[0];
        }
        else if (abs(value_) < 0.6f)
        {
            description += adjectives_[1];
        }
        else if (abs(value_) < 1.0f)
        {
            description += adjectives_[2];
        }
        else
        {
            description += adjectives_[3];
        }
        description += fmt::format(" {}", GetNameVariant());
        return description;
    }
} // namespace tattletale