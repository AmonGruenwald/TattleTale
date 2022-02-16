#include "tale/talecore.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/actor.hpp"
#include <iostream>
#include <assert.h>
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
        TALE_DEBUG_PRINT("READ FROM PROTOTYPE FILE:\n");
        TALE_DEBUG_PRINT(prototype_json.dump(4) + "\n");
        TALE_DEBUG_PRINT("------------------------------------------\n\n");
        TALE_DEBUG_PRINT("CREATING INTERACTION PROTOTYPE CATALOGUE:\n");

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

            prototype_catalogue_.insert({interaction.name_, interaction});

            TALE_DEBUG_PRINT("\n" + interaction.ToString() + "\n");
        }
        TALE_DEBUG_PRINT("------------------------------------------\n\n");
    }
    std::string InteractionStore::GetRandomInteractionName()
    {
        std::vector<std::string> keys;
        for (const auto &[key, _] : prototype_catalogue_)
        {
            keys.push_back(key);
        }
        // TODO: use random value
        return keys[0];
    }
    const size_t &InteractionStore::GetParticipantCount(std::string interaction_name)
    {
        assert(prototype_catalogue_.count(interaction_name) == 1); // no interaction of that name
        return prototype_catalogue_.at(interaction_name).participant_count_;
    }
    const std::vector<float> &InteractionStore::GetResourceEffects(std::string interaction_name)
    {
        assert(prototype_catalogue_.count(interaction_name) == 1); // no interaction of that name
        return prototype_catalogue_.at(interaction_name).resource_effects_;
    }

    const std::vector<std::map<EmotionType, float>> &InteractionStore::GetEmotionEffects(std::string interaction_name)
    {
        assert(prototype_catalogue_.count(interaction_name) == 1); // no interaction of that name
        return prototype_catalogue_.at(interaction_name).emotion_effects_;
    }
    const std::vector<std::map<size_t, std::map<RelationshipType, float>>> &InteractionStore::GetRelationshipEffects(std::string interaction_name)
    {
        assert(prototype_catalogue_.count(interaction_name) == 1); // no interaction of that name
        return prototype_catalogue_.at(interaction_name).relationship_effects_;
    }
    std::shared_ptr<Interaction> InteractionStore::CreateInteraction(std::string interaction_name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants)
    {
        assert(prototype_catalogue_.count(interaction_name) == 1); // no interaction of that name
        Interaction &prototype = prototype_catalogue_.at(interaction_name);
        std::shared_ptr<Interaction> interaction = std::shared_ptr<Interaction>(new Interaction(
            prototype.name_,
            tick,
            reasons,
            prototype.participant_count_,
            participants,
            prototype.resource_effects_,
            prototype.emotion_effects_,
            prototype.relationship_effects_));

        return interaction;
    }
} // namespace tale