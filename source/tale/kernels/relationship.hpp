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
        kProtective,
        kLast
    };
    /**
     * @brief Represents a Relationship an Actor has with another Actor
     *
     */
    class Relationship : public Kernel
    {
    public:
        Relationship(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        static RelationshipType StringToRelationshipType(std::string relationship_string);
        static std::string RelationshipTypeToString(RelationshipType relationship_type);
        float GetValue() const;
        std::string ToString();

    private:
        float value_;
    };

} // namespace tale
#endif // TALE_RELATIONSHIP_H