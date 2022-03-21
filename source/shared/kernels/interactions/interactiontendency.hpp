#ifndef TALE_INTERACTIONS_INTERACTIONTENDENCY_H
#define TALE_INTERACTIONS_INTERACTIONTENDENCY_H

#include "shared/kernels/interactions/interactioncontexttype.hpp"
#include "shared/kernels/resourcekernels/resource.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include <map>
#include <fmt/core.h>
namespace tattletale
{
    /**
     * @brief TODO
     */
    struct InteractionTendency
    {
        std::map<ContextType, float> contexts = {
            {ContextType::kCourse, 0.0f},
            {ContextType::kFreetime, 0.0f}};
        float wealth = 0;
        std::vector<float> emotions = std::vector<float>(static_cast<int>(EmotionType::kLast), 0.0f);
        std::vector<std::vector<float>> relationships = {};

        std::string ToString()
        {
            std::string contexts_string = "Contexts:\n";
            for (auto &[key, value] : contexts)
            {
                contexts_string += fmt::format("\t{}: {}\n", ContextTypeToString(key), value);
            }
            std::string wealth_string = fmt::format("Wealth: {}\n", wealth);
            std::string emotions_string = "Emotions:\n";
            for (int type_index = 0; type_index < static_cast<int>(EmotionType::kLast); ++type_index)
            {
                EmotionType type = static_cast<EmotionType>(type_index);
                emotions_string += fmt::format("\t{}: {}\n", Emotion::EmotionTypeToString(type), emotions[type_index]);
            }
            std::string relationship_string = "Relationships:\n";

            for (size_t i = 0; i < relationships.size(); ++i)
            {
                relationship_string += fmt::format("\tFor Participant{}:\n", i + 1);
                for (int type_index = 0; type_index < static_cast<int>(RelationshipType::kLast); ++type_index)
                {
                    RelationshipType type = static_cast<RelationshipType>(type_index);
                    relationship_string += fmt::format("\t\t{}: {}\n", Relationship::RelationshipTypeToString(type), relationships[i][type_index]);
                }
            }
            return (fmt::format("{}\n{}\n{}\n{}", contexts_string, wealth_string, emotions_string, relationship_string));
        }
        void ClearValues()
        {
            contexts.clear();
            wealth = 0;
            emotions = std::vector<float>(static_cast<int>(EmotionType::kLast));
            relationships.clear();
        }
    };

} // namespace tattletale
#endif // TALE_INTERACTIONS_INTERACTIONTENDENCY_H