#ifndef TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H
#define TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H

#include <fmt/core.h>
#include "shared/kernels/interactions/interactioncontexttype.hpp"
#include "shared/kernels/goal.hpp"
namespace tattletale
{
    /**
     * @brief Stores all necessary \link InteractionRequirement Requirements \endlink for an Interaction to happen.
     */
    struct InteractionRequirement
    {
        /**
         * @brief ContextType the corresponding Interaction needs.
         */
        ContextType context = ContextType::kLast;
        /**
         * @brief How many \link Actor Actors \endlink the corresponding Interaction needs.
         */
        size_t participant_count = 1;
        GoalType goal_type = GoalType::kLast;
        size_t day = 0;
        std::vector<float> emotions = std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f);
        std::vector<float> relationship = std::vector<float>(static_cast<int>(RelationshipType::kLast), 0.0f);

        void ClearValues()
        {
            context = ContextType::kLast;
            participant_count = 1;
            goal_type = GoalType::kLast;
            day = 0;
            emotions = std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f);
            relationship = std::vector<float>(static_cast<int>(RelationshipType::kLast), 0.0f);
        }

        bool HasEmotionalRequirement() const
        {
            for (const auto &value : emotions)
            {
                if (value != 0)
                {
                    return true;
                }
            }
            return false;
        }

        bool HasRelationshipRequirement() const
        {
            for (const auto &value : relationship)
            {
                if (value != 0)
                {
                    return true;
                }
            }
            return false;
        }
    };

} // namespace tattletale

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::InteractionRequirement, T>::value, char>> : fmt::formatter<std::string>
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
    auto format(tattletale::InteractionRequirement &requirement, FormatContext &ctx) -> decltype(ctx.out())
    {
        std::string string = fmt::format(
            "Context: {} \
                \nParticipant Count: {} \
                \nGoal Type: {} \
                \n Day: {} \
                \nEmotions: \
                \n\tHappy: {} \
                \n\tCalme: {} \
                \n\tSatisfied: {} \
                \n\tBrave: {} \
                \n\tExtroverted: {} \
                \nRelationship: \
                \n\tLove: {} \
                \n\tAttraction: {} \
                \n\tFriendship: {} \
                \n\tAnger: {} \
                \n\tProtective: {} ",
            requirement.context,
            requirement.participant_count,
            requirement.goal_type,
            requirement.day,
            requirement.emotions[static_cast<int>(tattletale::EmotionType::kHappy)],
            requirement.emotions[static_cast<int>(tattletale::EmotionType::kCalm)],
            requirement.emotions[static_cast<int>(tattletale::EmotionType::kSatisfied)],
            requirement.emotions[static_cast<int>(tattletale::EmotionType::kBrave)],
            requirement.emotions[static_cast<int>(tattletale::EmotionType::kExtroverted)],
            requirement.relationship[static_cast<int>(tattletale::RelationshipType::kLove)],
            requirement.relationship[static_cast<int>(tattletale::RelationshipType::kAttraction)],
            requirement.relationship[static_cast<int>(tattletale::RelationshipType::kFriendship)],
            requirement.relationship[static_cast<int>(tattletale::RelationshipType::kAnger)],
            requirement.relationship[static_cast<int>(tattletale::RelationshipType::kProtective)]);
        return fmt::formatter<std::string>::format(string, ctx);
    }
};
#endif // TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H