#ifndef TALE_GLOBALS_INTERACTIONSTORE_H
#define TALE_GLOBALS_INTERACTIONSTORE_H

#include <string>
#include <memory>
#include "shared/kernels/interactions/interaction.hpp"
#include "shared/kernels/interactions/interactionprototype.hpp"
#include "shared/kernels/interactions/interactionrequirement.hpp"
#include "shared/kernels/interactions/interactiontendency.hpp"
#include "shared/random.hpp"
#include "shared/chronicle.hpp"
#include "shared/tattletalecore.hpp"
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <robin_hood.h>

namespace tattletale
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
         * Reads from tale/resources/interactioncatalogue.json to get the data to build up the interaction prototype catalogue.
         *
         * @param random Reference to the Random object used by the simulation.
         */
        InteractionStore(Random &random);
        /**
         * @brief Returns a random interactionPrototype index from the catalogue.
         *
         * @return A random interactionPrototype index
         */
        uint32_t GetRandomInteractionPrototypeIndex() const;
        /**
         * @brief Returns the name of the Interaction that would be created with the passed index.
         *
         * @param prototype_index The index of the corresponding InteractionPrototype.
         * @return The name of the Interaction.
         */
        std::string GetInteractionName(size_t prototype_index) const;
        /**
         * @brief Returns the participation count for a catalogued InteractionPrototype.
         *
         * Participation count is needed to know how many \link Actor Actors \endlink are necessary for creation of an
         * Interaction.
         * This will fail if the index is invalid.
         * @param prototype_index Index of the queried InteractionPrototype.
         * @return The participation count
         */
        const size_t &GetParticipantCount(size_t prototype_index) const;
        /**
         * @brief Returns the wealth effects for a catalogued InteractionPrototype.
         *
         * Should only really be needed for testing purposes
         * @param prototype_index Index of the queried InteractionPrototype.
         * @return The wealth effects
         */
        const std::vector<float> &GetWealthEffects(size_t prototype_index) const;
        /**
         * @brief Returns the emotion effects for a catalogued InteractionPrototype.
         *
         * Should only really be needed for testing purposes
         * @param prototype_index Index of the queried InteractionPrototype.
         * @return The emotion effects
         */
        const std::vector<std::vector<float>> &GetEmotionEffects(size_t prototype_index) const;
        /**
         * @brief Returns the relationship effects for a catalogued InteractionPrototype.
         *
         * Should only really be needed for testing purposes
         * @param prototype_index Index of the queried InteractionPrototype.
         * @return The relationship effects
         */
        const std::vector<robin_hood::unordered_map<size_t, std::vector<float>>> &GetRelationshipEffects(size_t prototype_index) const;
        /**
         * @brief Creates an interaction from a catalogued InteractionPrototype.
         *
         * This is the only way an Interaction should be created. This ensures that the member variables of the Interaction is properly
         * populated.
         * @param prototype_index Index of the InteractionPrototype that should be used as a template
         * @param tick Tick this interaction was created
         * @param participants Vectors holding every Actor that was involved in the Interaction
         * @param reasons Vectors holding every Kernel responsible for the creation.
         * @return A pointer to the created Interaction
         */
        Interaction *CreateInteraction(Chronicle &chronicle, size_t prototype_index, float chance, size_t tick, std::vector<Kernel *> reasons, std::vector<Actor *> participants);
        /**
         * @brief Getter for a Reference to the  InteractionRequirement catalogue.
         *
         * @return A Reference to the catalogue for the \link InteractionRequirement Requirements \endlink.
         */
        const std::vector<std::shared_ptr<InteractionRequirement>> &GetRequirementCatalogue() const;
        /**
         * @brief Getter for a Reference to the InteractionTendency catalogue.
         *
         * @return A Reference to the catalogue for the \link InteractionTendency Tendencies \endlink.
         */
        const std::vector<std::shared_ptr<InteractionTendency>> &GetTendencyCatalogue() const;

    private:
        /**
         * @brief Holds a reference to the Random object of the simulation.
         */
        Random &random_;
        /**
         * @brief Holds all available \link InteractionPrototype InteractionPrototypes \endlink.
         */
        std::vector<std::shared_ptr<InteractionPrototype>> prototype_catalogue_;
        /**
         * @brief Holds the hard \link InteractionRequirement Requirements \endlink for all available Interaction prototypes.
         */
        std::vector<std::shared_ptr<InteractionRequirement>> requirements_catalogue_;
        /**
         * @brief Holds the hard \link InteractionRequirement Requirements \endlink for all available Interaction prototypes.
         */
        std::vector<std::shared_ptr<InteractionTendency>> tendencies_catalogue_;
        /**
         * @brief Path to the json file where Interaction prototypes are defined.
         */
        std::string prototype_json_path_ = "resources/interactioncatalogue.json";

        /**
         * @brief String mapping to name entries in the PrototypeCatalogue json.
         */
        const std::string name_key_ = "name";
        /**
         * @brief String mapping to description entries in the PrototypeCatalogue json.
         */
        const std::string description_key_ = "description";
        /**
         * @brief String mapping to passive_description entries in the PrototypeCatalogue json.
         */
        const std::string passive_description_key_ = "passive_description";
        /**
         * @brief String mapping to active_description entries in the PrototypeCatalogue json.
         */
        const std::string active_description_key_ = "active_description";
        /**
         * @brief String mapping to participant_count entries in the PrototypeCatalogue json.
         */
        const std::string participant_count_key_ = "participant_count";
        /**
         * @brief String mapping to goal_type entries in the PrototypeCatalogue json.
         */
        const std::string goal_type_key_ = "goal_type";
        /**
         * @brief String mapping to day entries in the PrototypeCatalogue json.
         */
        const std::string day_key_ = "day";
        /**
         * @brief String mapping to wealth entries in the PrototypeCatalogue json.
         */
        const std::string wealth_key_ = "wealth";
        /**
         * @brief String mapping to emotions entries in the PrototypeCatalogue json.
         */
        const std::string emotion_key_ = "emotions";
        /**
         * @brief String mapping to emotion type entries in the PrototypeCatalogue json.
         */
        const std::vector<std::string> emotion_type_keys_ = {"happy", "calm", "satisfied", "brave", "extroverted"};
        /**
         * @brief String mapping to context type entries in the PrototypeCatalogue json.
         */
        const std::vector<std::string> context_type_keys_ = {"course", "freetime"};
        /**
         * @brief String mapping to relationships entries in the PrototypeCatalogue json.
         */
        const std::string relationships_key_ = "relationships";
        /**
         * @brief String mapping to participant entries in the PrototypeCatalogue json.
         */
        const std::string participant_key_ = "participant";
        /**
         * @brief String mapping to changes entries in the PrototypeCatalogue json.
         */
        const std::string changes_key_ = "changes";
        /**
         * @brief String mapping to relationship type entries in the PrototypeCatalogue json.
         */
        const std::vector<std::string> relationship_type_keys_ = {"love", "attraction", "friendship", "anger", "protective"};
        /**
         * @brief String mapping to context entries in the PrototypeCatalogue json.
         */
        const std::string context_key_ = "context";
        /**
         * @brief String mapping to group_size entries in the PrototypeCatalogue json.
         */
        const std::string group_size_key_ = "group_size";

        /**
         * @brief Reads an InteractionPrototype from a json describing such.
         *
         * This also checks for various errors in the structure of the json.
         *
         * @param [in] json The json describing the InteractionPrototype.
         * @param [in] participant_count How many participants the Interaction to be read has.
         * @param [in] error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @param [out] out_prototype Stores the read InteractionPrototype.
         * @return Wether the process of reading was successful.
         */
        bool ReadPrototypeJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, std::shared_ptr<InteractionPrototype> &out_prototype);
        /**
         * @brief Reads an InteractionRequirement from a json describing such.
         *
         * This also checks for various errors in the structure of the json.
         *
         * @param [in] json The json describing the InteractionRequirement.
         * @param [in] error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @param [out] out_requirement Stores the read InteractionRequirement.
         * @return Wether the process of reading was successful.
         */
        bool ReadRequirementJSON(nlohmann::json json, std::string error_preamble, std::shared_ptr<InteractionRequirement> &out_requirement);
        /**
         * @brief Reads an InteractionTendency from a json describing such.
         *
         * This also checks for various errors in the structure of the json.
         *
         * @param [in] json The json describing the InteractionTendency.
         * @param [in] participant_count How many participants the Interaction to be read has.
         * @param [in] error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @param [out] out_tendency Stores the read InteractionTendency.
         * @return Wether the process of reading was successful.
         */
        bool ReadTendencyJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, std::shared_ptr<InteractionTendency> &out_tendency);

        /**
         * @brief Reads a value of type T from a json object mapping to a dictionary.
         *
         * This also checks wether the value was existant (if it is required) and wether it was of the correct type T2.
         *
         * @param [out] out_value Stores the read value.
         * @param [in] json The json object from which the value will be read. Must be an array.
         * @param [in] key The key that corresponds to the value that will be read.
         * @param [in] required Wether the value is required to be existant.
         * @param [in] error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @return Wether the process of reading was successful.
         */
        template <typename T1, nlohmann::detail::value_t T2>
        bool ReadJsonValueFromDictionary(T1 &out_value, nlohmann::json json, std::string key, bool required, std::string error_preamble = "")
        {
            if (!json.contains(key))
            {
                if (required)
                {
                    TATTLETALE_ERROR_PRINT(true, fmt::format("{}JSON DID NOT CONTAIN KEY {}", error_preamble, key));
                    return false;
                }
            }
            else
            {
                if (json[key].type() == T2)
                {
                    out_value = json[key];
                }
                else
                {
                    TATTLETALE_ERROR_PRINT(true, fmt::format("{}VALUE FOR KEY {} WAS OF AN INCORRECT TYPE", error_preamble, key));
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Reads a value of type T from a json object mapping to an array.
         *
         * This also checks wether the value was existant (if it is required) and wether it was of the correct type T2.
         *
         * @param [out] out_value Stores the read value.
         * @param [in] json The json object from which the value will be read. Must be an array.
         * @param [in] index The index from where the value will be read.
         * @param [in] required Wether the value is required to be existant.
         * @param [in] error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @return Wether the process of reading was successful.
         */
        template <typename T1, nlohmann::detail::value_t T2>
        bool ReadJsonValueFromArray(T1 &out_value, nlohmann::json json, size_t index, bool required, std::string error_preamble = "")
        {
            if (json.size() <= index)
            {
                if (required)
                {
                    TATTLETALE_ERROR_PRINT(true, fmt::format("{}JSON DID NOT CONTAIN INDEX {}", error_preamble, index));
                    return false;
                }
            }
            else
            {
                if (json[index].type() == T2)
                {
                    out_value = json[index];
                }
                else
                {
                    TATTLETALE_ERROR_PRINT(true, fmt::format("{}VALUE FOR INDEX {} WAS OF AN INCORRECT TYPE", error_preamble, index));
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Checks wether the passed value is between the range of -1.0f and 1.0f.
         *
         * @param value The value to be checked.
         * @param error_preamble A string that will be prepended to the error message if the value is outside the range.
         * @return The result of the check.
         */
        bool CheckCorrectValueRange(float value, std::string error_preamble = "");
    };

} // namespace tattletale
#endif // TALE_GLOBALS_INTERACTIONSTORE_H