#ifndef TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H
#define TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H

#include <string>
#include <vector>
#include <map>
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include <fmt/core.h>

namespace tattletale
{
    /**
     * @brief Prototype of an Interaction.
     *
     * Used by the InteractionStore to hold basic information and effects of an Interaction.
     */
    struct InteractionPrototype
    {
        /**
         * @brief The name of the corresponding Interaction.
         */
        std::string name = "";
        /**
         * @brief The description template of the corresponding Interaction.
         */
        std::string description = "";
        /**
         * @brief Stores the effect the corresponding Interaction has on each participating \link Actor Actor's \endlink wealth.
         */
        std::vector<float> wealth_effects;
        /**
         * @brief Stores the effect the corresponding Interaction Interaction has on each participating \link Actor Actor's \endlink \link Emotion Emotions \endlink.
         */
        std::vector<std::map<EmotionType, float>> emotion_effects;
        /**
         * @brief TODO Stores the effect the corresponding Interaction has on each participating \link Actor Actor's \endlink \link Relationship Relationships \endlink.
         *
         * For each Actor a map is created that maps to participants index in the participants_ vector another map of RelationshipType float pairs.
         * Those pairs signal how much each Relationship axis would be changed.
         */
        std::vector<std::map<size_t, std::map<RelationshipType, float>>> relationship_effects;

        /**
         * @brief Creates a string describing the InteractionPrototype.
         *
         * @return The descriptive string.
         */
        std::string ToString()
        {
            std::string name_string = fmt::format("Name: {}\n", name);
            std::string wealth_effects_string = "";
            for (size_t i = 0; i < wealth_effects.size(); ++i)
            {
                wealth_effects_string += fmt::format("\t{}. Wealth Effect: {}\n", i, wealth_effects[i]);
            }
            std::string emotion_effects_string = "";
            for (size_t i = 0; i < emotion_effects.size(); ++i)
            {
                emotion_effects_string += fmt::format("\t{}. Emotion Effect:", i);
                for (auto &[emotion_type, emotion_value] : emotion_effects[i])
                {
                    emotion_effects_string += fmt::format("\n\t\t{}: {}", Emotion::EmotionTypeToString(emotion_type), emotion_value);
                }
                emotion_effects_string += "\n";
            }
            std::string relationship_effects_string = "";
            for (size_t i = 0; i < relationship_effects.size(); ++i)
            {
                relationship_effects_string += fmt::format("\tRelationship Effects for Participant {}:", i);
                for (auto &[other_participant, relationship_map] : relationship_effects[i])
                {
                    relationship_effects_string += fmt::format("\n\t\tWith Participant {}:", other_participant);
                    for (auto &[relationship_type, relationship_value] : relationship_effects[i][other_participant])
                    {
                        relationship_effects_string += fmt::format("\n\t\t\t{}: {}", Relationship::RelationshipTypeToString(relationship_type), relationship_value);
                    }
                }
                relationship_effects_string += "\n";
            }
            return (name_string + "\n" + wealth_effects_string + "\n" + emotion_effects_string + "\n" + relationship_effects_string);
        }

        void ClearValues()
        {
            name = "";
            wealth_effects.clear();
            emotion_effects.clear();
            relationship_effects.clear();
        }
    };

} // namespace tattletale
#endif // TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H