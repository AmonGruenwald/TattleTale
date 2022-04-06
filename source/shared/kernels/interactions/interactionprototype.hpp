#ifndef TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H
#define TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H

#include <string>
#include <vector>
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include <fmt/core.h>
#include <robin_hood.h>

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
         * @brief The id of the InteractionPrototype.
         */
        size_t id = 0;
        /**
         * @brief The description template of the corresponding Interaction.
         */
        std::string description = "";
        /**
         * @brief The passive description of this protoype.
         */
        std::string passive_description = "";
        /**
         * @brief The active description of this protoype.
         */
        std::string active_description = "";
        /**
         * @brief Stores the effect the corresponding Interaction has on each participating \link Actor Actor's \endlink wealth.
         */
        std::vector<float> wealth_effects;
        /**
         * @brief Stores the effect the corresponding Interaction Interaction has on each participating \link Actor Actor's \endlink \link Emotion Emotions \endlink.
         */
        std::vector<std::vector<float>> emotion_effects;
        /**
         * @brief Stores the effect the corresponding Interaction has on each participating \link Actor Actor's \endlink \link Relationship Relationships \endlink.
         *
         * For each Actor a map is created that maps an index in the participants_ vector to another vector of floats, which indices correspond to the RelationshipType enum.
         * Those pairs signal how much each Relationship axis would be changed.
         */
        std::vector<robin_hood::unordered_map<size_t, std::vector<float>>> relationship_effects;

        /**
         * @brief Reset all values back to their default state.
         */
        void ClearValues()
        {
            name = "";
            wealth_effects.clear();
            emotion_effects.clear();
            relationship_effects.clear();
        }
    };

} // namespace tattletale

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::InteractionPrototype, T>::value, char>> : fmt::formatter<std::string>
{
    // d - default
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'd'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(tattletale::InteractionPrototype &prototype, FormatContext &ctx) -> decltype(ctx.out())
    {
        std::string name_string = fmt::format("Name: {}\n", prototype.name);
        std::string wealth_effects_string = "";
        for (size_t i = 0; i < prototype.wealth_effects.size(); ++i)
        {
            wealth_effects_string += fmt::format("\t{}. Wealth Effect: {}\n", i, prototype.wealth_effects[i]);
        }
        std::string emotion_effects_string = "";
        for (size_t i = 0; i < prototype.emotion_effects.size(); ++i)
        {
            emotion_effects_string += fmt::format("\t{}. Emotion Effect:", i);

            for (int type_index = 0; type_index < prototype.emotion_effects[i].size(); ++type_index)
            {
                float value = prototype.emotion_effects[i][type_index];
                tattletale::EmotionType type = static_cast<tattletale::EmotionType>(type_index);
                emotion_effects_string += fmt::format("\n\t\t{}: {}", type, value);
            }
            emotion_effects_string += "\n";
        }
        std::string relationship_effects_string = "";
        for (size_t i = 0; i < prototype.relationship_effects.size(); ++i)
        {
            relationship_effects_string += fmt::format("\tRelationship Effects for Participant {}:", i);
            for (auto &[other_participant, relationship_vector] : prototype.relationship_effects[i])
            {
                relationship_effects_string += fmt::format("\n\t\tWith Participant {}:", other_participant);
                for (int type_index = 0; type_index < relationship_vector.size(); ++type_index)
                {
                    tattletale::RelationshipType type = static_cast<tattletale::RelationshipType>(type_index);
                    relationship_effects_string += fmt::format("\n\t\t\t{}: {}", type, relationship_vector[type_index]);
                }
            }
            relationship_effects_string += "\n";
        }
        return fmt::formatter<std::string>::format(name_string + "\n" + wealth_effects_string + "\n" + emotion_effects_string + "\n" + relationship_effects_string, ctx);
    }
};
#endif // TALE_INTERACTIONS_INTERACTIONPROTOTYPE_H