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

namespace tattletale
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

        std::string GetDefaultDescription() const override;
        std::string GetDetailedDescription() const override;
        std::string GetPassiveDescription() const override;
        std::string GetActiveDescription() const override;

        /**
         * @brief Getter for the InteractionPrototype this Interaction uses.
         *
         * @return The InteractionPrototype.
         */
        const std::shared_ptr<InteractionPrototype> GetPrototype() const;
        const std::shared_ptr<InteractionTendency> GetTendency() const;
        /**
         * @brief Getter for the participants this Interaction uses.
         *
         * @return The participants.
         */
        const std::vector<Actor *> &GetParticipants() const;

        float GetChance() const override;

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
            const std::shared_ptr<InteractionPrototype> Prototype,
            const std::shared_ptr<InteractionRequirement> requirement,
            const std::shared_ptr<InteractionTendency> tendency,
            float chance,
            size_t id,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            std::vector<Actor *>
                participants);
        /**
         * @brief Stores a Reference to the corresponding InteractionPrototype.
         */
        const std::shared_ptr<InteractionPrototype> prototype_;
        const std::shared_ptr<InteractionRequirement> requirement_;
        const std::shared_ptr<InteractionTendency> tendency_;
        float chance_;
        /**
         * @brief Stores Pointers to the involved \link Actor Actors \endlink.
         */
        std::vector<Actor *>
            participants_;
        friend class Chronicle;
    };
} // namespace tattletale

#endif // TALE_KERNELS_INTERACTIONS_INTERACTION_H