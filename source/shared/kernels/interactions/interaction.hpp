#ifndef TALE_KERNELS_INTERACTIONS_INTERACTION_H
#define TALE_KERNELS_INTERACTIONS_INTERACTION_H

#include "shared/kernels/kernel.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/kernels/interactions/interactionprototype.hpp"
#include "shared/kernels/interactions/interactionrequirement.hpp"
#include "shared/kernels/interactions/interactiontendency.hpp"
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
    class Interaction : public Kernel
    {
    public:
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
        const std::vector<std::weak_ptr<Actor>> &GetParticipants() const;

    private:
        /**
         * @brief Only publically available Constructor. Fills out every member variable.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param prototype Reference to the InteractionPrototype this Interaction is based upon.
         * @param requirement Reference to the InteractionRequirement this Interaction uses.
         * @param tendency Reference to the InteractionTendency this Interaction uses.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick The tick during which this Interaction happens.
         * @param reasons Vector of \link Kernel Kernels \endlink that are responsible for this Interaction happening.
         * @param participants Vector of \link Actor Actors \endlink that are participating in this Interaction.
         **/
        Interaction(
            const InteractionPrototype &prototype,
            const InteractionRequirement &requirement,
            const InteractionTendency &tendency,
            float chance,
            size_t id,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            std::vector<std::weak_ptr<Actor>>
                participants);
        /**
         * @brief Stores a Reference to the corresponding InteractionPrototype.
         */
        const InteractionPrototype &prototype_;
        const InteractionRequirement &requirement_;
        const InteractionTendency &tendency;
        float chance_;
        /**
         * @brief Stores Pointers to the involved \link Actor Actors \endlink.
         */
        std::vector<std::weak_ptr<Actor>>
            participants_;
        friend class Chronicle;
    };

} // namespace tale
#endif // TALE_KERNELS_INTERACTIONS_INTERACTION_H