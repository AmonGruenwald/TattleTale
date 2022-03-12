#include "tale/kernels/resourcekernels/relationship.hpp"

#include <assert.h>
#include <iostream>
#include <fmt/core.h>
#include "tale/actor.hpp"

namespace tale
{
    Relationship::Relationship(RelationshipType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::weak_ptr<Actor> target, std::vector<std::weak_ptr<Kernel>> reasons, float value)
        : Resource(RelationshipTypeToString(type), id, tick, owner, reasons, value), target_(target), type_(type){};

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
        assert(false); // invalid string was passed
        return RelationshipType::kNone;
    }
    std::string Relationship::RelationshipTypeToString(RelationshipType relationship_type)
    {
        switch (relationship_type)
        {
        case RelationshipType::kNone:
            assert(false); // invalid enum was passed
            return "none";
            break;
        case RelationshipType::kLast:
            assert(false); // invalid enum was passed
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
    std::string Relationship::ToString()
    {
        return fmt::format("{} felt {} with a value of {} for {}", owner_.lock()->name_, name_, value_, target_.lock()->name_);
    }
} // namespace tale