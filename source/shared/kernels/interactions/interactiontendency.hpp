#ifndef TALE_INTERACTIONS_INTERACTIONTENDENCY_H
#define TALE_INTERACTIONS_INTERACTIONTENDENCY_H

#include "shared/kernels/interactions/interactioncontexttype.hpp"
#include "shared/kernels/resourcekernels/resource.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include <fmt/core.h>
namespace tattletale
{
    /**
     * @brief Stores how the state of \link Actor Actors \endlink influence how likely an Interaction is to happen.
     */
    struct InteractionTendency
    {
        /**
         * @brief Vector of floats, which indices map to the ContextType enum, that stores how the current Context might influence the choice.
         */
        std::vector<float> contexts = std::vector<float>(static_cast<int>(ContextType::kLast), 0.0f);

        /**
         * @brief Stores how the wealth of the Actor influences the choice.
         */
        float wealth = 0;
        /**
         * @brief Stores how the wealth of the Actor influences the choice.
         */
        std::vector<float> emotions = std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f);
        /**
         * @brief Stores how the relationship to each participant influences how likely the Actor is to pick them for this role.
         */
        std::vector<std::vector<float>> relationships = {};

        /**
         * @brief Reset all values back to their default state.
         */
        void ClearValues()
        {
            contexts = std::vector<float>(static_cast<int>(ContextType::kLast), 0.0f);
            wealth = 0;
            emotions = std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f);
            relationships.clear();
        }
    };

} // namespace tattletale

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::InteractionTendency, T>::value, char>> : fmt::formatter<std::string>
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
    auto format(tattletale::InteractionTendency &tendency, FormatContext &ctx) -> decltype(ctx.out())
    {
        std::string contexts_string = "Contexts:\n";
        for (int type_index = 0; type_index < static_cast<int>(tattletale::ContextType::kLast); ++type_index)
        {
            tattletale::ContextType type = static_cast<tattletale::ContextType>(type_index);
            contexts_string += fmt::format("\t{}: {}\n", type, tendency.contexts[type_index]);
        }
        std::string wealth_string = fmt::format("Wealth: {}\n", tendency.wealth);
        std::string emotions_string = "Emotions:\n";
        for (int type_index = 0; type_index < static_cast<int>(tattletale::EmotionType::kLast); ++type_index)
        {
            tattletale::EmotionType type = static_cast<tattletale::EmotionType>(type_index);
            emotions_string += fmt::format("\t{}: {}\n", type, tendency.emotions[type_index]);
        }
        std::string relationship_string = "Relationships:\n";

        for (size_t i = 0; i < tendency.relationships.size(); ++i)
        {
            relationship_string += fmt::format("\tFor Participant{}:\n", i + 1);
            for (int type_index = 0; type_index < static_cast<int>(tattletale::RelationshipType::kLast); ++type_index)
            {
                tattletale::RelationshipType type = static_cast<tattletale::RelationshipType>(type_index);
                relationship_string += fmt::format("\t\t{}: {}\n", type, tendency.relationships[i][type_index]);
            }
        }

        return fmt::formatter<std::string>::format(fmt::format("{}\n{}\n{}\n{}", contexts_string, wealth_string, emotions_string, relationship_string), ctx);
    }
};
#endif // TALE_INTERACTIONS_INTERACTIONTENDENCY_H