#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

namespace tale
{
    InteractionStore::InteractionStore()
    {
        std::ifstream prototype_json_file(prototype_json_path_);
        nlohmann::json prototype_json;
        prototype_json_file >> prototype_json;
        prototype_json_file.close();
        std::cout << "READ FROM PROTOTYPE FILE:" << std::endl;
        std::cout << prototype_json.dump(4) << std::endl;
        std::cout << "------------------------------------------" << std::endl
                  << std::endl;

        std::cout << "CREATING INTERACTION PROTOTYPE CATALOGUE:" << std::endl;

        for (auto &[key, value] : prototype_json.items())
        {
            Interaction interaction;

            interaction.name_ = key;

            size_t participant_count = value["participant_count"];
            interaction.participant_count_ = participant_count;

            auto effects = value["effects"];

            interaction.resource_effects_.reserve(participant_count);
            for (auto &resource : effects["resources"])
            {
                interaction.resource_effects_.push_back(resource);
            }

            interaction.emotion_effects_.reserve(participant_count);
            for (auto &emotions : effects["emotions"])
            {
                std::map<EmotionType, float> emotions_changes_map;
                for (auto &[emotion_key, emotion_change] : emotions.items())
                {
                    EmotionType type = Emotion::StringToEmotionType(emotion_key);
                    emotions_changes_map.insert({type, emotion_change});
                }
                interaction.emotion_effects_.push_back(emotions_changes_map);
            }

            interaction.relationship_effects_.reserve(participant_count);
            for (auto &relationships : effects["relationships"])
            {
                std::map<size_t, std::map<RelationshipType, float>> relationship_per_participant_change_map;
                for (auto &change_per_participant : relationships)
                {
                    size_t participant = change_per_participant["participant"];
                    std::map<RelationshipType, float> relationship_change_map;
                    for (auto &[relationship_key, relationship_change] : change_per_participant["changes"].items())
                    {
                        RelationshipType type = Relationship::StringToRelationshipType(relationship_key);
                        relationship_change_map.insert({type, relationship_change});
                    }
                    relationship_per_participant_change_map.insert({participant, relationship_change_map});
                }
                interaction.relationship_effects_.push_back(relationship_per_participant_change_map);
            }

            std::cout << std::endl
                      << interaction.ToString() << std::endl;
        }

        std::cout << "------------------------------------------" << std::endl
                  << std::endl;
    }
    std::string InteractionStore::GetRandomInteractionName()
    {
        return "test";
    }
    Interaction InteractionStore::CloneInteraction()
    {
        return Interaction();
    }
} // namespace tale