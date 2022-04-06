#ifndef TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H
#define TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H

#include <fmt/core.h>
#include "shared/kernels/interactions/interactioncontexttype.hpp"
#include "shared/kernels/goal.hpp"
namespace tattletale
{
    /**
     * @brief Stores all necessary requirements for an Interaction to happen.
     */
    struct InteractionRequirement
    {
        /**
         * @brief ContextType the corresponding Interaction needs for it to happen.
         */
        ContextType context = ContextType::kLast;
        /**
         * @brief How many \link Actor Actors \endlink the corresponding Interaction needs.
         */
        size_t participant_count = 1;
        /**
         * @brief The goal the active Actor needs to try to fulfill.
         */
        GoalType goal_type = GoalType::kLast;
        /**
         * @brief After which day the interaction can first happen
         */
        size_t day = 0;
        /**
         * @brief What emotions the participating Actors have to have for the Interaction to happen.
         */
        std::vector<std::vector<float>> emotions;

        /**
         * @brief What relationship the Actor has to have with the participants he chooses.
         */
        std::vector<std::vector<float>> relationship;

        /**
         * @brief Reset all values back to their default state.
         */
        void ClearValues()
        {
            context = ContextType::kLast;
            participant_count = 1;
            goal_type = GoalType::kLast;
            day = 0;
            emotions.clear();
            relationship.clear();
        }

        void SetParticipantCount(size_t participant_count)
        {
            this->participant_count = participant_count;
            for (size_t i = 0; i < participant_count; ++i)
            {
                emotions.push_back(std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f));
            }
            for (size_t active_participant = 1; active_participant < participant_count; ++active_participant)
            {
                relationship.push_back(std::vector<float>(static_cast<int>(RelationshipType::kLast), 0.0f));
            }
        }
        /**
         * @brief Checks wether an \link Emotion Emotional \endlink requirement exists.
         * @return The result of the check.
         */
        bool HasEmotionalRequirement(size_t participant_index) const
        {
            for (const auto &value : emotions[participant_index])
            {
                if (value != 0)
                {
                    return true;
                }
            }
            return false;
        }
        /**
         * @brief Checks wether an Relationship requirement exists.
         * @return The result of the check.
         */
        bool HasRelationshipRequirement(size_t participant_index) const
        {
            if (participant_count == 1)
            {
                return false;
            }
            for (const auto &value : relationship[participant_index - 1])
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
        std::string string = fmt::format("Context: {}", requirement.context);
        string += fmt::format("\nParticipant Count: {}", requirement.participant_count);
        string += fmt::format("\nGoal Type: {}", requirement.goal_type);
        string += fmt::format("\nDay: {}", requirement.day);
        string += "\nEmotions:";
        for (size_t i = 0; i < requirement.emotions.size(); ++i)
        {
            string += fmt::format("\n\tParticipant: {}", i);
            for (int type_index = 0; type_index < requirement.emotions[i].size(); ++type_index)
            {
                string += fmt::format(
                    "\n\t\t{:<11}: {}",
                    static_cast<tattletale::EmotionType>(type_index),
                    requirement.emotions[i][type_index]);
            }
        }
        string += "\nRelationship:";
        for (size_t i = 0; i < requirement.relationship.size(); ++i)
        {
            string += fmt::format("\n\twith Participant: {}", i + 1);
            for (int type_index = 0; type_index < requirement.relationship[i].size(); ++type_index)
            {
                string += fmt::format(
                    "\n\t\t\t{:<10}: {}",
                    static_cast<tattletale::RelationshipType>(type_index),
                    requirement.relationship[i][type_index]);
            }
        }
        return fmt::formatter<std::string>::format(string, ctx);
    }
};
#endif // TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H