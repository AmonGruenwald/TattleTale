#ifndef TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H
#define TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H

#include <fmt/core.h>
#include "tale/kernels/interactions/interactioncontexttype.hpp"
#include "tale/kernels/goal.hpp"
namespace tale
{
    /**
     * @brief Stores all necessary \link InteractionRequirement Requirements \endlink for an Interaction to happen.
     */
    struct InteractionRequirement
    {
        /**
         * @brief ContextType the corresponding Interaction needs.
         */
        ContextType context = ContextType::kNone;
        /**
         * @brief How many \link Actor Actors \endlink the corresponding Interaction needs.
         */
        size_t participant_count = 1;
        GoalType goal_type = GoalType::kNone;
        size_t day = 0;
        std::map<EmotionType, float> emotions = {
            {EmotionType::kHappy, 0.0f},
            {EmotionType::kCalm, 0.0f},
            {EmotionType::kSatisfied, 0.0f},
            {EmotionType::kBrave, 0.0f},
            {EmotionType::kExtroverted, 0.0f}};
        std::map<RelationshipType, float> relationship = {
            {RelationshipType::kLove, 0.0f},
            {RelationshipType::kAttraction, 0.0f},
            {RelationshipType::kFriendship, 0.0f},
            {RelationshipType::kAnger, 0.0f},
            {RelationshipType::kProtective, 0.0f}};

        std::string ToString()
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
                ContextTypeToString(context),
                participant_count,
                Goal::GoalTypeToString(goal_type),
                day,
                emotions.at(EmotionType::kHappy),
                emotions.at(EmotionType::kCalm),
                emotions.at(EmotionType::kSatisfied),
                emotions.at(EmotionType::kBrave),
                emotions.at(EmotionType::kExtroverted),
                relationship.at(RelationshipType::kLove),
                relationship.at(RelationshipType::kAttraction),
                relationship.at(RelationshipType::kFriendship),
                relationship.at(RelationshipType::kAnger),
                relationship.at(RelationshipType::kProtective));
            return string;
        }

        void ClearValues()
        {
            context = ContextType::kNone;
            participant_count = 1;
            goal_type = GoalType::kNone;
            day = 0;
            emotions.clear();
            relationship.clear();
        }

        bool HasEmotionalRequirement() const
        {
            for (const auto &[key, value] : emotions)
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
            for (const auto &[key, value] : relationship)
            {
                if (value != 0)
                {
                    return true;
                }
            }
            return false;
        }
    };

} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H