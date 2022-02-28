#ifndef TALE_INTERACTIONS_INTERACTIONTENDENCY_H
#define TALE_INTERACTIONS_INTERACTIONTENDENCY_H

#include "tale/kernels/interactions/interactioncontexttype.hpp"
#include "tale/kernels/resourcekernels/resource.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include <map>
#include <fmt/core.h>
namespace tale
{
    /**
     * @brief TODO
     */
    struct Tendency
    {
        float group_size = 0;
        std::map<ContextType, float> contexts = {
            {ContextType::kCourse, 0.0f},
            {ContextType::kFreetime, 0.0f}};
        float wealth = 0;
        std::map<EmotionType, float> emotions = {
            {EmotionType::kHappy, 0.0f},
            {EmotionType::kCalm, 0.0f},
            {EmotionType::kSatisfied, 0.0f},
            {EmotionType::kBrave, 0.0f},
            {EmotionType::kExtroverted, 0.0f}};
        std::vector<std::map<RelationshipType, float>> relationships = {};

        std::string ToString()
        {
            std::string grout_size_string = fmt::format("Group Size: {}\n", group_size);
            std::string contexts_string = "Contexts:\n";
            for (auto &[key, value] : contexts)
            {
                contexts_string += fmt::format("\t{}: {}\n", ContextTypeToString(key), value);
            }
            std::string wealth_string = fmt::format("Wealth: {}\n", wealth);
            std::string emotions_string = "Emotions:\n";
            for (auto &[key, value] : emotions)
            {
                emotions_string += fmt::format("\t{}: {}\n", Emotion::EmotionTypeToString(key), value);
            }
            std::string relationship_string = "Relationships:\n";

            for (size_t i = 0; i < relationships.size(); ++i)
            {
                relationship_string += fmt::format("\tFor Participant{}:\n", i + 1);
                for (auto &[key, value] : relationships[i])
                {
                    relationship_string += fmt::format("\t\t{}: {}\n", Relationship::RelationshipTypeToString(key), value);
                }
            }
            return (fmt::format("{}\n{}\n{}\n{}\n{}", grout_size_string, contexts_string, wealth_string, emotions_string, relationship_string));
        }
        void ClearValues()
        {
            group_size = 0;
            contexts.clear();
            wealth = 0;
            emotions.clear();
            relationships.clear();
        }
    };

} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONTENDENCY_H