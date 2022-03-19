#include "shared/kernels/resourcekernels/resource.hpp"

#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tattletale
{
    Resource::Resource(
        std::string name,
        std::string positive_name_variant,
        std::string negative_name_variant,
        size_t id,
        size_t tick,
        Actor *owner,
        std::vector<Kernel *> reasons,
        float value,
        KernelType type,
        Verb verb)
        : Kernel(name, id, tick, owner, reasons, type),
          positive_name_variant_(positive_name_variant),
          negative_name_variant_(negative_name_variant),
          value_(value),
          verb_(verb){};
    float Resource::GetValue() const
    {
        return value_;
    }
    std::string Resource::GetNameVariant() const
    {
        if (value_ < 0)
        {
            return negative_name_variant_;
        }
        return positive_name_variant_;
    }
    std::string Resource::GetAdjective() const
    {
        if (abs(value_) < 0.3f)
        {
            return "slightly";
        }
        else if (abs(value_) < 0.6f)
        {
            return "quite";
        }
        else if (abs(value_) < 1.0f)
        {
            return "very";
        }
        return "completely";
    }

    std::string Resource::GetDetailedDescription() const
    {
        return fmt::format("{} with value of {}", Kernel::GetDetailedDescription(), value_);
    }
    std::string Resource::GetDefaultDescription() const
    {
        return fmt::format("{:d} {} {} {}", verb_, owner_->name_, GetAdjective(), GetNameVariant());
    }

    std::string Resource::GetPassiveDescription() const
    {
        return fmt::format("{:p} {} {}", verb_, GetAdjective(), GetNameVariant());
        ;
    }

    std::string Resource::GetActiveDescription() const
    {
        return fmt::format("{:a} {} {}", verb_, GetAdjective(), GetNameVariant());
    }

} // namespace tattletale