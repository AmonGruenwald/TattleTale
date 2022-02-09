#ifndef TALE_INTERACTION_H
#define TALE_INTERACTION_H

#include "tale/kernels/kernel.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/actor.hpp"

namespace tale
{
    /**
     * @brief Represents an Interaction an Actor had with other \link Actor Actors \endlink
     *
     * Extends the Kernel class, so it has to hold every information that is necessary to recreate the causal
     * chain leading to this Event.
     */
    class Interaction : public Kernel
    {
    public:
        /**
         * @brief Stores how many \link Actor Actors \endlink were involved.
         *
         * This is variable is only necessary because prototypes in InteractionStore cannot yet hold actual
         * \link Actor Actors \endlink, but need to store how many are needed.
         */
        size_t participant_count_;

        /**
         * @brief Stores Pointers to the involved \link Actor Actors \endlink.
         */
        std::vector<std::weak_ptr<Actor>> participants_;

        /**
         * @brief Stores the effect this Interaction has on each participating \link Actor Actor's \endlink \link Resource Resources \endlink.
         */
        std::vector<float> resource_effects_;
        /**
         * @brief Stores the effect this Interaction has on each participating \link Actor Actor's \endlink \link Emotion Emotions \endlink.
         */
        std::vector<std::map<EmotionType, float>> emotion_effects_;
        /**
         * @brief Stores the effect this Interaction has on each participating \link Actor Actor's \endlink \link Relationship Relationships \endlink.
         *
         * For each Actor a map is created that maps to participants index in the participants_ vector another map of RelationshipType float pairs.
         * Those pairs signal how much each Relationship axis would be changed.
         */

        std::vector<std::map<size_t, std::map<RelationshipType, float>>> relationship_effects_;

        /**
         * @brief Only publically available Constructor. Fills out every member variable.
         *
         * Even though it is publically available, this probably should only be called from InteractionStore. It is only publically available because it
         * might be needed for testing purposes.
         */
        Interaction(
            std::string name,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            size_t participant_count,
            std::vector<std::weak_ptr<Actor>> participants,
            std::vector<float> resource_effects,
            std::vector<std::map<EmotionType, float>> emotion_effects,
            std::vector<std::map<size_t, std::map<RelationshipType, float>>> relationship_effects);

        /**
         * @brief Gets a string representation of the Interaction.
         *
         * @return String describing the status of the object
         */
        std::string ToString();

    private:
        /**
         * @brief Default constructor should never be called.
         *
         * This only exists so InteractionStore can create the prototypes.
         */
        Interaction();

        friend class InteractionStore;
    };

} // namespace tale
#endif // TALE_INTERACTION_H