#ifndef TALE_INTERACTION_H
#define TALE_INTERACTION_H

#include "tale/kernels/kernel.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include <map>
#include <memory>

namespace tale
{
    class Actor;
    /**
     * @brief Represents an Interaction an Actor had with other \link Actor Actors \endlink
     *
     * Extends the Kernel class, so it has to hold every information that is necessary to recreate the causal
     * chain leading to this Event.
     */
    class Interaction : public Kernel, public std::enable_shared_from_this<Interaction>
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
         * @param name The name of this Interaction.
         * @param tick The tick during which this Interaction happens.
         * @param reasons Vector of \link Kernel Kernels \endlink that are responsible for this Interaction happening.
         * @param participant_count How many \link Actor Actors \endlink are participating in this Interaction. Seperate from participants because
         * protoypes of Interactions do not actually have participants
         * @param participants Vector of \link Actor Actors \endlink that are participating in this Interaction.
         * @param resource_effects Vector holding the effects this Interaction will have on the Resource of the participating \link Actor Actors \endlink.
         * @param emotion_effects Vector holding the effects this Interaction will have on the \link Emotion Emotions \endlink  of the participating \link Actor Actors \endlink.
         * Maps Emotiontype to float value.
         * @param relationship_effects Vector holding the effects this Interaction will have on the \link Relationship Relationships \endlink  of the
         * participating \link Actor Actors \endlink. Maps index of Actor in participants_ vector to map of RelationshipType to float value.
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
         * @brief Applies the effects of the Interaction to all participating \link Actor Actors \endlink.
         */
        void Apply();
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