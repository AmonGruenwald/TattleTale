#include "shared/kernels/resourcekernels/relationship.hpp"
#include <assert.h>
#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tattletale
{
    Relationship::Relationship(RelationshipType type, size_t id, size_t tick, Actor *owner, Actor *target, std::vector<Kernel *> reasons, float value)
        : Resource(
              fmt::format("{}", type),
              Relationship::positive_name_variants_[static_cast<int>(type)],
              Relationship::negative_name_variants_[static_cast<int>(type)],
              id,
              tick,
              owner,
              reasons,
              value,
              KernelType::kRelationship,
              Verb("felt", "feeling", "feel")),
          target_(target),
          type_(type){};

    RelationshipType Relationship::StringToRelationshipType(std::string relationship_string)
    {
        if (relationship_string == "love")
            return RelationshipType::kLove;
        if (relationship_string == "attraction")
            return RelationshipType::kAttraction;
        if (relationship_string == "friendship")
            return RelationshipType::kFriendship;
        if (relationship_string == "anger")
            return RelationshipType::kAnger;
        if (relationship_string == "protective")
            return RelationshipType::kProtective;

        TATTLETALE_ERROR_PRINT(true, "Invalid Relationship string was passed");
        return RelationshipType::kLast;
    }

    std::string Relationship::GetAdjective() const
    {
        if ((type_ == RelationshipType::kProtective && value_ >= 0) || (type_ == RelationshipType::kAnger && value_ < 0))
        {
            return Resource::GetAdjective();
        }
        else
        {
            if (abs(value_) < 0.3f)
            {
                return "slight";
            }
            else if (abs(value_) < 0.6f)
            {
                return "";
            }
            else if (abs(value_) < 1.0f)
            {
                return "a lot of";
            }
            return "all encompassing";
        }
    }

    std::string Relationship::GetDefaultDescription() const
    {
        return fmt::format("{} {}", Resource::GetDefaultDescription(), *target_);
    }
    std::string Relationship::GetDetailedDescription() const
    {
        return fmt::format("{} targeting {}", Resource::GetDetailedDescription(), *target_);
    }
    std::string Relationship::GetPassiveDescription() const
    {
        return fmt::format("{} {}", Resource::GetPassiveDescription(), *target_);
    }
    std::string Relationship::GetActiveDescription() const
    {
        return fmt::format("{} {}", Resource::GetActiveDescription(), *target_);
    }
} // namespace tattletale