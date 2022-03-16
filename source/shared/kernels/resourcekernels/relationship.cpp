#include "shared/kernels/resourcekernels/relationship.hpp"

#include <assert.h>
#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tattletale
{
    Relationship::Relationship(RelationshipType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::weak_ptr<Actor> target, std::vector<std::weak_ptr<Kernel>> reasons, float value)
        : Resource(
              RelationshipTypeToString(type),
              Relationship::positive_name_variants_.at(type),
              Relationship::negative_name_variants_.at(type),
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
        return RelationshipType::kNone;
    }
    std::string Relationship::RelationshipTypeToString(RelationshipType relationship_type)
    {
        switch (relationship_type)
        {
        case RelationshipType::kNone:
            TATTLETALE_ERROR_PRINT(true, "Invalid Relationship type was passed");
            return "none";
            break;
        case RelationshipType::kLast:
            TATTLETALE_ERROR_PRINT(true, "Invalid Relationship type was passed");
            return "last";
            break;
        case RelationshipType::kLove:
            return "love";
            break;
        case RelationshipType::kAttraction:
            return "attraction";
            break;
        case RelationshipType::kFriendship:
            return "friendship";
            break;
        case RelationshipType::kAnger:
            return "anger";
            break;
        case RelationshipType::kProtective:
            return "protective";
            break;
        }
        return "none";
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

    //  "passive_description" : "talking about the weather",
    //                         "active_description" : "talk about the weather with {0}",
    std::string Relationship::GetDefaultDescription() const
    {
        return fmt::format("{} {}", Resource::GetDefaultDescription(), target_.lock()->name_);
    }
    std::string Relationship::GetPassiveDescription() const
    {
        return fmt::format("{} {}", Resource::GetPassiveDescription(), target_.lock()->name_);
    }
    std::string Relationship::GetActiveDescription() const
    {
        return fmt::format("{} {}", Resource::GetActiveDescription(), target_.lock()->name_);
    }
} // namespace tattletale