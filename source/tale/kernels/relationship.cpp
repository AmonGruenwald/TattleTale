#include "tale/kernels/relationship.hpp"

#include <assert.h>
#include <iostream>

namespace tale
{
    Relationship::Relationship(std::string name, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, reasons){};

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
        assert(relationship_type != RelationshipType::kNone); // invalid enum was passed
        switch (relationship_type)
        {
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
        return "I am Relationship.\n";
    }
} // namespace tale