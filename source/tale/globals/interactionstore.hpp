#ifndef TALE_GLOBALS_INTERACTIONSTORE_H
#define TALE_GLOBALS_INTERACTIONSTORE_H

#include <string>
#include <map>
#include <memory>
#include "tale/kernels/interaction.hpp"

namespace tale
{
    class Actor;
    /**
     * @brief Stores Prototypes for all possible interactions.
     *
     * An Interaction should only be created through this class, so it's field are correctly filled.
     * Also contains helper function to get appropriate Interaction options for each situation
     */
    class InteractionStore
    {
    public:
        /**
         * @brief Default constructor populates catalogue.
         *
         * Reads from tale/resources/interactionprototypes.json to get the data to build up the interaction catalogue.
         */
        InteractionStore();
        /**
         * @brief Returns a random interaction name from the catalogue.
         *
         * @return A random Interaction name
         */
        std::string GetRandomInteractionName();
        /**
         * @brief Returns the participation count for a catalogued Interaction name.
         *
         * Participation count is needed to know how many \link Actor Actors \endlink are necessary for creation of the
         * Interaction.
         * This Will fail if the name is invalid.
         * @return The participation count
         */
        const size_t &GetParticipantCount(std::string interaction_name);
        /**
         * @brief Returns the resource effects for a catalogued Interaction name.
         *
         * @param interaction_name Name of the queried Interaction prototype
         * Should only really be needed for testing purposes
         * @return The resource effects
         */
        const std::vector<float> &GetResourceEffects(std::string interaction_name);
        /**
         * @brief Returns the emotion effects for a catalogued Interaction name.
         *
         * @param interaction_name Name of the queried Interaction prototype
         * Should only really be needed for testing purposes
         * @return The emotion effects
         */
        const std::vector<std::map<EmotionType, float>> &GetEmotionEffects(std::string interaction_name);
        /**
         * @brief Returns the relationship effects for a catalogued Interaction name.
         *
         * @param interaction_name Name of the queried Interaction prototype
         * Should only really be needed for testing purposes
         * @return The relationship effects
         */
        const std::vector<std::map<size_t, std::map<RelationshipType, float>>> &GetRelationshipEffects(std::string interaction_name);
        /**
         * @brief Creates an interaction from a catalogued prototype.
         *
         * @param interaction_name Name of the Interaction prototype that should be used as a template
         * @param tick Tick this interaction was created
         * @param participants Vectors holding every Actor that was involved in the Interaction
         * This is the only way an Interaction should be created. This ensures that the member variables of the Interaction is properly
         * populated.
         * @return A pointer to the created Interaction
         */
        std::shared_ptr<Interaction> CreateInteraction(std::string interaction_name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants);

    private:
        /**
         * @brief Holds all available Interaction prototypes.
         */
        std::map<std::string, Interaction> prototype_catalogue_;
        /**
         * @brief Path to the json file where Interaction prototypes are defined.
         */
        std::string prototype_json_path_ = "tale/resources/interactionprototypes.json";
    };

} // namespace tale
#endif // TALE_GLOBALS_INTERACTIONSTORE_H