#ifndef TALE_KERNELS_RESOURCEKERNELS_RELATIONSHIP_H
#define TALE_KERNELS_RESOURCEKERNELS_RELATIONSHIP_H

#include "shared/kernels/resourcekernels/resource.hpp"

namespace tattletale
{
    /**
     * @brief Types of Relationship axis for an Actor.
     */
    enum class RelationshipType
    {
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
     * Will be instantiated inside a map connecting different other \link Actor Actors \endlink to a Vector containing
     * \link Relationship Relationships \endlink for each Actor mapped to indices converted from their RelationshipType.
     *
     */
    class Relationship : public Resource
    {
    public:
        /**
         * @brief Converts a string to an RelationshipType.
         *
         * Uses the same string values formater returns.
         * So valid strings are: love", "attraction", "friendship", "anger" and "protective".
         * Everything else just return RelationshipType::kLast. (But an assert will crash the Application in Debug mode.)
         *
         * @param relationship_string The string we want to convert.
         * @return The corresponding RelationshipType value.
         */
        static RelationshipType StringToRelationshipType(std::string relationship_string);

        /**
         * @brief Gets the default description for the Relationship.
         *
         * This description forms a standalone sentence. E.g.: "John Doe felt a lot of love for Jane Doe"
         * @return The default description.
         */
        std::string GetDefaultDescription() const override;
        /**
         * @brief Gets a detailed description for the Relationship.
         *
         * This is only used for debugging purposes and should not be incorporated into curation as it is not in the
         * form of a well structured sentence.
         * @return The detailed description.
         */
        std::string GetDetailedDescription() const override;
        /**
         * @brief Gets a passive description for the Relationship.
         *
         * This description forms a passive sentence fragment that can be incorporated into a narrative.
         * E.g.: "feeling a lot of love for Jane Doe"
         * @return The passive description.
         */
        std::string GetPassiveDescription() const override;
        /**
         * @brief Gets an active description for the Relationship.
         *
         * This description forms an active sentence fragment that can be incorporated into a narrative.
         * E.g.: "feel a lot of love for Jane Doe"
         * @return The passive description.
         */
        std::string GetActiveDescription() const override;

    private:
        /**
         * @brief Constructor initializing base Resource class and type_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param type What RelationshipType this Relationship is of.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Relationship was created.
         * @param owner The Actor which owns this Resource.
         * @param target The Actor which is the passive recipient of thisvRelationship.
         * @param reasons What other \link Kernel Kernels \endlink led to this Relationship and its value.
         * @param value Value of the Relationship between -1.0 and 1.0.
         */
        Relationship(RelationshipType type, size_t id, size_t tick, Actor *owner, Actor *target, std::vector<Kernel *> reasons, float value);
        /**
         * @brief Getter to find an adjective describing the severity of this emotion.
         *
         * @return The appropriate adjective.
         */
        std::string GetAdjective() const override;
        /**
         * @brief The RelationshipType of this Relationship.
         */
        RelationshipType type_;
        /**
         * @brief Vector containing the positive name variants for this emotion.
         */
        const static inline std::vector<std::string> positive_name_variants_ =
            {"love for",
             "attraction for",
             "friendship for",
             "anger for",
             "protective of"};
        /**
         * @brief Vector containing the negative name variants for this emotion.
         */
        const static inline std::vector<std::string> negative_name_variants_ =
            {"hate for",
             "disgust for",
             "animosity for",
             "comfortable with",
             "power over for"};
        /**
         * @brief The Actor which is targeted by this Relationship.
         */
        Actor *target_;
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
        friend class Chronicle;
    };

} // namespace tattletale
template <>
struct fmt::formatter<tattletale::RelationshipType> : formatter<string_view>
{
    std::string relationship_type_names[6] = {
        "love",
        "attraction",
        "friendship",
        "anger",
        "protective",
        "last"};
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(tattletale::RelationshipType type, FormatContext &ctx)
    {
        size_t enum_index = static_cast<size_t>(type);
        string_view name = relationship_type_names[enum_index];
        return formatter<string_view>::format(name, ctx);
    }
};
#endif // TALE_KERNELS_RESOURCEKERNELS_RELATIONSHIP_H