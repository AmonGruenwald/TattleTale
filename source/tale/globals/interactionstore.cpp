#include "tale/talecore.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include "tale/actor.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>

namespace tale
{
    InteractionStore::InteractionStore(Random &random, Chronicle &chronicle) : random_(random), chronicle_(chronicle)
    {
        TALE_VERBOSE_PRINT("READING FROM PROTOTYPE FILE");
        std::ifstream catalogue_json_file(prototype_json_path_);
        nlohmann::json catalogue_json;
        catalogue_json_file >> catalogue_json;
        catalogue_json_file.close();
        TALE_VERBOSE_PRINT(catalogue_json.dump(4) + "\n\n");
        TALE_DEBUG_PRINT("CREATING INTERACTION PROTOTYPE CATALOGUE");

        size_t interaction_number = 0;
        for (auto &interaction : catalogue_json)
        {
            TALE_DEBUG_PRINT("DESERIALIZING CATALOGUE INFO #" + std::to_string(interaction_number));

            InteractionRequirement requirement;
            std::string requirement_error_preamble = fmt::format("REQUIREMENT {}: ", interaction_number);
            if (ReadRequirementJSON(interaction["requirements"], requirement_error_preamble, requirement))
            {
                InteractionPrototype prototype;
                std::string prototype_error_preamble = fmt::format("REQUIREMENT #{}: ", interaction_number);
                if (ReadPrototypeJSON(interaction["prototype"], requirement.participant_count, prototype_error_preamble, prototype))
                {
                    InteractionTendency tendency;
                    std::string tendency_error_preamble = fmt::format("TENDENCY {}: ", interaction_number);
                    if (ReadTendencyJSON(interaction["tendencies"], requirement.participant_count, tendency_error_preamble, tendency))
                    {
                        prototype_catalogue_.push_back(prototype);
                        requirements_catalogue_.push_back(requirement);
                        tendencies_catalogue_.push_back(tendency);
                    }
                }
            }

            ++interaction_number;
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
    std::weak_ptr<Interaction> InteractionStore::CreateInteraction(size_t prototype_index, float chance, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants)
    {
        assert(prototype_catalogue_.size() > prototype_index); // faulty index
        InteractionPrototype &prototype = prototype_catalogue_.at(prototype_index);
        InteractionRequirement &requirement = requirements_catalogue_.at(prototype_index);
        InteractionTendency &tendency = tendencies_catalogue_.at(prototype_index);
        return chronicle_.CreateInteraction(prototype, requirement, tendency, chance, tick, reasons, participants);
    }
    const std::vector<InteractionRequirement> &InteractionStore::GetRequirementCatalogue() const
    {
        return requirements_catalogue_;
    }
    const std::vector<InteractionTendency> &InteractionStore::GetTendencyCatalogue() const
    {
        return tendencies_catalogue_;
    }

    bool InteractionStore::ReadPrototypeJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, InteractionPrototype &out_prototype)
    {
        TALE_VERBOSE_PRINT("CREATING PROTOTYPE...");
        out_prototype.ClearValues();

        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype.name, json, name_key_, true, error_preamble))
        {
            return false;
        }

        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype.description, json, description_key_, true, error_preamble))
        {
            return false;
        }

        nlohmann::json wealth_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::array>(wealth_json, json, wealth_key_, false, error_preamble))
        {
            return false;
        }

        nlohmann::json emotion_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::array>(emotion_json, json, emotion_key_, false, error_preamble))
        {
            return false;
        }

        nlohmann::json relationship_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::array>(relationship_json, json, relationship_key_, false, error_preamble))
        {
            return false;
        }

        for (size_t i = 0; i < participant_count; ++i)
        {
            float wealth_value = 0.0f;
            if (!ReadJsonValueFromArray<float, nlohmann::detail::value_t::number_float>(wealth_value, wealth_json, i, false, error_preamble))
            {
                return false;
            }
            if (!CheckCorrectValueRange(wealth_value))
            {
                return false;
            }
            out_prototype.wealth_effects.push_back(wealth_value);

            nlohmann::json emotion_map;
            if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::object>(emotion_map, emotion_json, i, false, error_preamble))
            {
                return false;
            }
            out_prototype.emotion_effects.push_back({});
            for (auto &key : emotion_type_keys_)
            {
                float emotion_value = 0.0f;
                if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(emotion_value, emotion_map, key, false, error_preamble))
                {
                    return false;
                }
                if (!CheckCorrectValueRange(emotion_value))
                {
                    return false;
                }
                out_prototype.emotion_effects[i].insert({Emotion::StringToEmotionType(key), emotion_value});
            }

            nlohmann::json relationship_changes_array;
            if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::array>(relationship_changes_array, relationship_json, i, false, error_preamble))
            {
                return false;
            }
            size_t index = 0;
            out_prototype.relationship_effects.push_back({});
            for (size_t other_participant = 0; other_participant < participant_count; ++other_participant)
            {
                if (other_participant != i)
                {
                    nlohmann::json relationship_change_map;
                    if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::object>(relationship_change_map, relationship_changes_array, index, false, error_preamble))
                    {
                        return false;
                    }
                    size_t participant = other_participant;
                    if (!ReadJsonValueFromDictionary<size_t, nlohmann::detail::value_t::number_unsigned>(participant, relationship_change_map, participant_key_, false, error_preamble))
                    {
                        return false;
                    }
                    if (participant >= participant_count)
                    {
                        TALE_ERROR_PRINT(fmt::format("{} PARTICIPANT INDEX FOR PROTOTYPE {} WAS TO BIG", error_preamble, out_prototype.name));
                        return false;
                    }
                    nlohmann::json changes_json;
                    if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(changes_json, relationship_change_map, changes_key_, false, error_preamble))
                    {
                        return false;
                    }
                    std::map<RelationshipType, float> map;
                    for (auto &key : relationship_type_keys_)
                    {
                        float value = 0.0f;
                        if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(value, changes_json, key, false, error_preamble))
                        {
                            return false;
                        }
                        if (!CheckCorrectValueRange(value))
                        {
                            return false;
                        }
                        map.insert({Relationship::StringToRelationshipType(key), value});
                    }

                    out_prototype.relationship_effects[i].insert({participant, map});
                    ++index;
                }
            }
        }

        TALE_VERBOSE_PRINT("CREATED INTERACTION PROTOTYPE:\n" + out_prototype.ToString() + "\n");

        return true;
    }

    bool InteractionStore::ReadRequirementJSON(nlohmann::json json, std::string error_preamble, InteractionRequirement &out_requirement)
    {
        TALE_DEBUG_PRINT("CREATING REQUIREMENTS...");
        out_requirement.ClearValues();
        if (!ReadJsonValueFromDictionary<size_t, nlohmann::detail::value_t::number_unsigned>(out_requirement.participant_count, json, participant_count_key_, true, error_preamble))
        {
            return false;
        }
        std::string context_value = "";
        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(context_value, json, context_key_, false, error_preamble))
        {
            return false;
        }
        out_requirement.context = StringToContextType(context_value);

        std::string goal_type_value = "";
        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(goal_type_value, json, goal_type_key_, false, error_preamble))
        {
            return false;
        }
        out_requirement.goal_type = Goal::StringToGoalType(goal_type_value);

        if (!ReadJsonValueFromDictionary<size_t, nlohmann::detail::value_t::number_unsigned>(out_requirement.day, json, day_key_, false, error_preamble))
        {
            return false;
        }

        nlohmann::json emotion_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(emotion_json, json, emotion_key_, false, error_preamble))
        {
            return false;
        }
        for (auto &key : emotion_type_keys_)
        {
            float emotion_value = 0.0f;
            if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(emotion_value, emotion_json, key, false, error_preamble))
            {
                return false;
            }
            if (!CheckCorrectValueRange(emotion_value))
            {
                return false;
            }
            out_requirement.emotions.insert({Emotion::StringToEmotionType(key), emotion_value});
        }
        nlohmann::json relationship_map_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(relationship_map_json, json, relationship_key_, false, error_preamble))
        {
            return false;
        }
        for (auto &key : relationship_type_keys_)
        {
            float relationship_value = 0.0f;
            if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(relationship_value, relationship_map_json, key, false, error_preamble))
            {
                return false;
            }
            if (!CheckCorrectValueRange(relationship_value))
            {
                return false;
            }
            out_requirement.relationship.insert({Relationship::StringToRelationshipType(key), relationship_value});
        }

        TALE_VERBOSE_PRINT("CREATED INTERACTION REQUIREMENT:\n" + out_requirement.ToString() + "\n");
        return true;
    }

    bool InteractionStore::ReadTendencyJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, InteractionTendency &out_tendency)
    {
        out_tendency.ClearValues();
        TALE_VERBOSE_PRINT("CREATING TENDENCY...");

        nlohmann::json context_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(context_json, json, context_key_, false, error_preamble))
        {
            return false;
        }
        for (auto &key : context_type_keys_)
        {
            float context_value = 0.0f;
            if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(context_value, context_json, key, false, error_preamble))
            {
                return false;
            }
            if (!CheckCorrectValueRange(context_value))
            {
                return false;
            }
            out_tendency.contexts.insert({StringToContextType(key), context_value});
        }
        float wealth_value = 0.0f;
        if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(wealth_value, json, wealth_key_, false, error_preamble))
        {
            return false;
        }
        if (!CheckCorrectValueRange(wealth_value))
        {
            return false;
        }
        out_tendency.wealth = wealth_value;

        nlohmann::json emotion_json;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(emotion_json, json, emotion_key_, false, error_preamble))
        {
            return false;
        }
        for (auto &key : emotion_type_keys_)
        {
            float emotion_value = 0.0f;
            if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(emotion_value, emotion_json, key, false, error_preamble))
            {
                return false;
            }
            if (!CheckCorrectValueRange(emotion_value))
            {
                return false;
            }
            out_tendency.emotions.insert({Emotion::StringToEmotionType(key), emotion_value});
        }
        nlohmann::json relationship_array;
        if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::array>(relationship_array, json, relationship_key_, false, error_preamble))
        {
            return false;
        }
        for (size_t i = 0; i < participant_count - 1; ++i)
        {
            nlohmann::json relationship_map_json;
            if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::object>(relationship_map_json, relationship_array, i, false, error_preamble))
            {
                return false;
            }
            std::map<RelationshipType, float> relationship_map;
            for (auto &key : relationship_type_keys_)
            {
                float relationship_value = 0.0f;
                if (!ReadJsonValueFromDictionary<float, nlohmann::detail::value_t::number_float>(relationship_value, relationship_map_json, key, false, error_preamble))
                {
                    return false;
                }
                if (!CheckCorrectValueRange(relationship_value))
                {
                    return false;
                }
                relationship_map.insert({Relationship::StringToRelationshipType(key), relationship_value});
            }
            out_tendency.relationships.push_back(relationship_map);
        }

        TALE_VERBOSE_PRINT("CREATED INTERACTION TENDENCY:\n" + out_tendency.ToString() + "\n");
        return true;
    }

    bool InteractionStore::CheckCorrectValueRange(float value, std::string error_preamble)
    {
        float min = -1.0f;
        float max = 1.0f;
        if (value > max)
        {
            TALE_ERROR_PRINT(fmt::format("{}VALUE OF {} IS BIGGER THAN {}", error_preamble, value, max));
            return false;
        }
        if (value < min)
        {
            TALE_ERROR_PRINT(fmt::format("{}VALUE OF {} IS SMALLER THAN {}", error_preamble, value, min));
            return false;
        }
        return true;
    }

} // namespace tale