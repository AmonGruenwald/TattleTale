#ifndef TALE_KERNELS_INTERACTIONS_INTERACTION_H
#define TALE_KERNELS_INTERACTIONS_INTERACTION_H

#include "shared/kernels/kernel.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/kernels/interactions/interactionprototype.hpp"
#include "shared/kernels/interactions/interactionrequirement.hpp"
#include "shared/kernels/interactions/interactiontendency.hpp"
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

        /**
         * @brief Gets the default description for the Interaction.
         *
         * The template for this description is defined in interactioncatalogue.json and takes the
         * form of a standalone sentence. E.g.: "John Doe did an interaction with Jane Doe"
         * @return The default description.
         */
        std::string GetDefaultDescription() const override;
        /**
         * @brief Gets a detailed description for the Interaction.
         *
         * This is only used for debugging purposes and should not be incorporated into curation as it is not in the
         * form of a well structured sentence.
         * @return The detailed description.
         */
        std::string GetDetailedDescription() const override;
        /**
         * @brief Gets a passive description for the Interaction.
         *
         * The template for this description is defined in interactioncatalogue.json and takes the
         * form of a passive sentence fragment that can be integrated into a narrative template.
         * E.g.: "doing an interaction"
         * @return The passive description.
         */
        std::string GetPassiveDescription() const override;
        /**
         * @brief Gets an active description for the Interaction.
         *
         * The template for this description is defined in interactioncatalogue.json and takes the
         * form of an active sentence fragment that can be integrated into a narrative template.
         * E.g.: "do an interaction"
         * @return The active description.
         */
        std::string GetActiveDescription() const override;

        /**
         * @brief Getter for the InteractionPrototype this Interaction uses.
         *
         * @return The InteractionPrototype.
         */
        const std::shared_ptr<InteractionPrototype> GetPrototype() const;
        /**
         * @brief Getter for the InteractionTendency this Interaction uses.
         *
         * @return The InteractionPrototype.
         */
        const std::shared_ptr<InteractionTendency> GetTendency() const;

        /**
         * @brief Getter for the participants this Interaction uses.
         *
         * @return The participants.
         */
        const std::vector<Actor *> &GetParticipants() const;

        /**
         * @brief Overriden getter for the chance this Interaction had when it was chosen.
         *
         * Interactions are chosen randomly, so they are the only Kernel where this function does not just
         * return 1.0f.
         *
         * @return The chance.
         */
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
         * @param chance The chance this Interaction had to be chosen.
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
            std::vector<Kernel *> reasons,
            std::vector<Actor *> participants);

        /**
         * @brief Stores a Reference to the corresponding InteractionPrototype.
         */
        const std::shared_ptr<InteractionPrototype> prototype_;
        /**
         * @brief Stores a Reference to the corresponding InteractionRequirement.
         */
        const std::shared_ptr<InteractionRequirement> requirement_;
        /**
         * @brief Stores a Reference to the corresponding InteractionTendency.
         */
        const std::shared_ptr<InteractionTendency> tendency_;
        /**
         * @brief The chance this Interaction had to be chosen.
         */
        float chance_;
        /**
         * @brief Stores pointers to the involved \link Actor Actors \endlink.
         */
        std::vector<Actor *>
            participants_;
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
        friend class Chronicle;
    };
} // namespace tattletale

#endif // TALE_KERNELS_INTERACTIONS_INTERACTION_H