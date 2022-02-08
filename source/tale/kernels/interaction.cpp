#include "tale/kernels/interaction.hpp"

#include <fmt/core.h>
#include <iostream>

namespace tale
{
    Interaction::Interaction(std::string name, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, reasons){};
    Interaction::Interaction() : Kernel()
    {
        std::cout << "Interaction default constructor" << std::endl;
    };
    std::string Interaction::ToString()
    {
        std::string name_string = fmt::format("Name: {}\n", name_);
        std::string resource_effects_string = "";
        for (size_t i = 0; i < resource_effects_.size(); ++i)
        {
            resource_effects_string += fmt::format("\t{}. Resource Effect: {}\n", i, resource_effects_[i]);
        }
        std::string emotion_effects_string = "";
        for (size_t i = 0; i < emotion_effects_.size(); ++i)
        {
            emotion_effects_string += fmt::format("\t{}. Emotion Effect:", i);
            for (auto &[emotion_type, emotion_value] : emotion_effects_[i])
            {
                emotion_effects_string += fmt::format("\n\t\t{}: {}", Emotion::EmotionTypeToString(emotion_type), emotion_value);
            }
            emotion_effects_string += "\n";
        }
        std::string relationship_effects_string = "";
        for (size_t i = 0; i < relationship_effects_.size(); ++i)
        {
            relationship_effects_string += fmt::format("\tRelationship Effects for Participant {}:", i);
            for (auto &[other_participant, relationship_map] : relationship_effects_[i])
            {
                relationship_effects_string += fmt::format("\n\t\tWith Participant {}:", other_participant);
                for (auto &[relationship_type, relationship_value] : relationship_effects_[i][other_participant])
                {
                    relationship_effects_string += fmt::format("\n\t\t\t{}: {}", Relationship::RelationshipTypeToString(relationship_type), relationship_value);
                }
            }
            relationship_effects_string += "\n";
        }
        return (name_string + "\n" + resource_effects_string + "\n" + emotion_effects_string + "\n" + relationship_effects_string + "\n");
    }
} // namespace tale