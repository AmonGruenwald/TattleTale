#ifndef TALE_INTERACTIONS_INTERACTIONTENDENCY_H
#define TALE_INTERACTIONS_INTERACTIONTENDENCY_H

#include "tale/kernels/interactions/interactioncontexttype.hpp"
#include "tale/kernels/resourcekernels/resource.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include <map>
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
    };

} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONTENDENCY_H