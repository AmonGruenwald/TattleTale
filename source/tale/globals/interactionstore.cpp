#include "tale/talecore.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include "tale/actor.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include <nlohmann/json.hpp>

namespace tale
{
    InteractionStore::InteractionStore(Random &random, Chronicle &chronicle) : random_(random), chronicle_(chronicle)
    {
        TALE_DEBUG_PRINT("READING FROM PROTOTYPE FILE");
        std::ifstream catalogue_json_file(prototype_json_path_);
        nlohmann::json catalogue_json;
        catalogue_json_file >> catalogue_json;
        catalogue_json_file.close();
        TALE_VERBOSE_PRINT(catalogue_json.dump(4) + "\n\n");
        TALE_DEBUG_PRINT("CREATING INTERACTION PROTOTYPE CATALOGUE");

        for (auto &interaction : catalogue_json)
        {
            std::string name = interaction["prototype"]["name"];
            TALE_DEBUG_PRINT("DESERIALIZING CATALOGUE INFO FOR: " + name);

            TALE_DEBUG_PRINT("CREATING PROTOTYPE");

            auto prototype_json = interaction["prototype"];
            InteractionPrototype prototype;

            prototype.name = prototype_json["name"];

            for (auto &wealth : prototype_json["wealth"])
            {
                prototype.wealth_effects.push_back(wealth);
            }

            for (auto &emotions : prototype_json["emotions"])
            {
                std::map<EmotionType, float> emotions_changes_map;
                for (auto &[emotion_key, emotion_change] : emotions.items())
                {
                    EmotionType type = Emotion::StringToEmotionType(emotion_key);
                    emotions_changes_map.insert({type, emotion_change});
                }
                prototype.emotion_effects.push_back(emotions_changes_map);
            }

            for (auto &relationships : prototype_json["relationships"])
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
                prototype.relationship_effects.push_back(relationship_per_participant_change_map);
            }

            prototype_catalogue_.push_back(prototype);

            TALE_VERBOSE_PRINT("\nCREATED INTERACTION PROTOTYPE:\n" + prototype.ToString() + "\n");

            TALE_DEBUG_PRINT("CREATING REQUIREMENTS");

            auto requirement_json = interaction["requirements"];
            Requirement requirement;

            requirement.participant_count = requirement_json["participant_count"];

            requirement.context = StringToContextType(requirement_json["context"]);

            requirements_catalogue_.push_back(requirement);

            TALE_DEBUG_PRINT("CREATING TENDENCY");

            auto tendency_json = interaction["tendencies"];
            Tendency tendency;
            tendency.group_size = tendency_json["group_size"];
            for (auto &[type, value] : tendency_json["contexts"].items())
            {
                tendency.contexts[StringToContextType(type)] = value;
            }
            tendency.wealth = tendency_json["wealth"];
            for (auto &[type, value] : tendency_json["emotions"].items())
            {
                tendency.emotions[Emotion::StringToEmotionType(type)] = value;
            }
            for (auto relationship : tendency_json["relationships"])
            {
                std::map<RelationshipType, float> relationship_map;
                for (auto &[type, value] : relationship.items())
                {
                    relationship_map.insert({Relationship::StringToRelationshipType(type), value});
                }
                tendency.relationships.push_back(relationship_map);
            }

            tendencies_catalogue_.push_back(tendency);
        }
    }
    uint32_t InteractionStore::GetRandomInteractionPrototypeIndex() const
    {
        assert(prototype_catalogue_.size() > 0); // No interaction prototype created
        return random_.GetUInt(0, prototype_catalogue_.size() - 1);
    }
    std::string InteractionStore::GetInteractionName(size_t prototype_index) const
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        return prototype_catalogue_.at(prototype_index).name;
    }
    const size_t &InteractionStore::GetParticipantCount(size_t prototype_index) const
    {
        assert(requirements_catalogue_.size() > prototype_index); // faulty index
        return requirements_catalogue_.at(prototype_index).participant_count;
    }
    const std::vector<float> &InteractionStore::GetWealthEffects(size_t prototype_index) const
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        return prototype_catalogue_.at(prototype_index).wealth_effects;
    }

    const std::vector<std::map<EmotionType, float>> &InteractionStore::GetEmotionEffects(size_t prototype_index) const
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        return prototype_catalogue_.at(prototype_index).emotion_effects;
    }
    const std::vector<std::map<size_t, std::map<RelationshipType, float>>> &InteractionStore::GetRelationshipEffects(size_t prototype_index) const
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        return prototype_catalogue_.at(prototype_index).relationship_effects;
    }
    std::weak_ptr<Interaction> InteractionStore::CreateInteraction(size_t prototype_index, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants)
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        InteractionPrototype &prototype = prototype_catalogue_.at(prototype_index);
        return chronicle_.CreateInteraction(prototype, tick, reasons, participants);
    }
    const std::vector<Requirement> &InteractionStore::GetRequirementCatalogue() const
    {
        return requirements_catalogue_;
    }
    const std::vector<Tendency> &InteractionStore::GetTendencyCatalogue() const
    {
        return tendencies_catalogue_;
    }
} // namespace tale