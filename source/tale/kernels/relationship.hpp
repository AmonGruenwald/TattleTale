#ifndef TALE_RELATIONSHIP_H
#define TALE_RELATIONSHIP_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    enum class RelationshipType
    {
        kNone,
        kLove,
        kAttraction,
        kFriendship,
        kAnger,
        kProtective
    };
    /**
     * @brief Represents a Relationship an Actor has with another Actor
     *
     */
    class Relationship : public Kernel
    {
    public:
        Relationship(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        static RelationshipType StringToRelationshipType(std::string relationship_string);
        static std::string RelationshipTypeToString(RelationshipType relationship_type);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_RELATIONSHIP_H