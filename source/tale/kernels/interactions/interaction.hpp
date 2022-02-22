#ifndef TALE_KERNELS_INTERACTIONS_INTERACTION_H
#define TALE_KERNELS_INTERACTIONS_INTERACTION_H

#include "tale/kernels/kernel.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include "tale/kernels/interactions/interactionprototype.hpp"
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
         * @param wealth_effects Vector holding the effects this Interaction will have on the wealth of the participating \link Actor Actors \endlink.
         * @param emotion_effects Vector holding the effects this Interaction will have on the \link Emotion Emotions \endlink  of the participating \link Actor Actors \endlink.
         * Maps Emotiontype to float value.
         * @param relationship_effects Vector holding the effects this Interaction will have on the \link Relationship Relationships \endlink  of the
         * participating \link Actor Actors \endlink. Maps index of Actor in participants_ vector to map of RelationshipType to float value.
         */
        Interaction(
            const InteractionPrototype &prototype,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            std::vector<std::weak_ptr<Actor>> participants);

        /**
         * @brief Applies the effects of the Interaction to all participating \link Actor Actors \endlink.
         */
        void Apply();
        /**
         * @brief Gets a string description of what would happen during the Interaction.
         *
         * @return String describing the Interaction
         */
        std::string ToString();

        /**
         * @brief Getter for the InteractionPrototype this Interaction uses.
         *
         * @return The InteractionPrototype.
         */
        const InteractionPrototype &GetPrototype() const;
        /**
         * @brief Getter for the participants this Interaction uses.
         *
         * @return The participants.
         */
        const std::vector<std::weak_ptr<Actor>> &Interaction::GetParticipants() const;

    private:
        /**
         * @brief Stores a Reference to the corresponding InteractionPrototype.
         */
        const InteractionPrototype &prototype_;
        /**
         * @brief Stores Pointers to the involved \link Actor Actors \endlink.
         */
        std::vector<std::weak_ptr<Actor>> participants_;
    };

} // namespace tale
#endif // TALE_KERNELS_INTERACTIONS_INTERACTION_H