#include "shared/tattletalecore.hpp"
#include "tale/interactionstore.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include <iostream>
#include <assert.h>
#include <fstream>

namespace tattletale
{
    InteractionStore::InteractionStore(Random &random) : random_(random)
    {
        TATTLETALE_VERBOSE_PRINT("READING FROM PROTOTYPE FILE");
        std::ifstream catalogue_json_file(prototype_json_path_);
        nlohmann::json catalogue_json;
        catalogue_json_file >> catalogue_json;
        catalogue_json_file.close();
        TATTLETALE_VERBOSE_PRINT(catalogue_json.dump(4) + "\n\n");
        TATTLETALE_VERBOSE_PRINT("CREATING INTERACTION PROTOTYPE CATALOGUE");

        size_t interaction_id = 0;
        for (auto &interaction : catalogue_json)
        {
            TATTLETALE_VERBOSE_PRINT(fmt::format("DESERIALIZING CATALOGUE INFO #{}", interaction_id));

            std::shared_ptr<InteractionRequirement> requirement(new InteractionRequirement());
            std::string requirement_error_preamble = fmt::format("REQUIREMENT {}: ", interaction_id);
            if (ReadRequirementJSON(interaction["requirements"], requirement_error_preamble, requirement))
            {
                std::shared_ptr<InteractionPrototype> prototype(new InteractionPrototype());
                prototype->id = interaction_id;
                std::string prototype_error_preamble = fmt::format("REQUIREMENT #{}: ", interaction_id);
                if (ReadPrototypeJSON(interaction["prototype"], requirement->participant_count, prototype_error_preamble, prototype))
                {
                    std::shared_ptr<InteractionTendency> tendency(new InteractionTendency());
                    std::string tendency_error_preamble = fmt::format("TENDENCY {}: ", interaction_id);
                    if (ReadTendencyJSON(interaction["tendencies"], requirement->participant_count, tendency_error_preamble, tendency))
                    {
                        prototype_catalogue_.push_back(prototype);
                        requirements_catalogue_.push_back(requirement);
                        tendencies_catalogue_.push_back(tendency);
                    }
                }
            }

            ++interaction_id;
        }
    }
    uint32_t InteractionStore::GetRandomInteractionPrototypeIndex() const
    {
        TATTLETALE_ERROR_PRINT(prototype_catalogue_.size() != 0, "Interaction Catalogue is empty");
        return random_.GetUInt(0, prototype_catalogue_.size() - 1);
    }
    std::string InteractionStore::GetInteractionName(size_t prototype_index) const
    {
        TATTLETALE_ERROR_PRINT(prototype_index < prototype_catalogue_.size(), fmt::format("Prototype with id {} does not exist", prototype_index));
        return prototype_catalogue_.at(prototype_index)->name;
    }
    const size_t &InteractionStore::GetParticipantCount(size_t prototype_index) const
    {
        TATTLETALE_ERROR_PRINT(prototype_index < requirements_catalogue_.size(), fmt::format("Requirement with id {} does not exist", prototype_index));
        return requirements_catalogue_.at(prototype_index)->participant_count;
    }
    const std::vector<float> &InteractionStore::GetWealthEffects(size_t prototype_index) const
    {
        TATTLETALE_ERROR_PRINT(prototype_index < prototype_catalogue_.size(), fmt::format("Prototype with id {} does not exist", prototype_index));
        return prototype_catalogue_.at(prototype_index)->wealth_effects;
    }

    const std::vector<std::vector<float>> &InteractionStore::GetEmotionEffects(size_t prototype_index) const
    {
        TATTLETALE_ERROR_PRINT(prototype_index < prototype_catalogue_.size(), fmt::format("Prototype with id {} does not exist", prototype_index));
        return prototype_catalogue_.at(prototype_index)->emotion_effects;
    }
    const std::vector<std::map<size_t, std::vector<float>>> &InteractionStore::GetRelationshipEffects(size_t prototype_index) const
    {
        TATTLETALE_ERROR_PRINT(prototype_index < prototype_catalogue_.size(), fmt::format("Prototype with id {} does not exist", prototype_index));
        return prototype_catalogue_.at(prototype_index)->relationship_effects;
    }
    Interaction *InteractionStore::CreateInteraction(Chronicle &chronicle, size_t prototype_index, float chance, size_t tick, std::vector<Kernel *> reasons, std::vector<Actor *> participants)
    {
        // TODO: move this to chronicle
        TATTLETALE_ERROR_PRINT(prototype_index < prototype_catalogue_.size(), fmt::format("Prototype with id {} does not exist", prototype_index));
        std::shared_ptr<InteractionPrototype> prototype = prototype_catalogue_.at(prototype_index);
        std::shared_ptr<InteractionRequirement> &requirement = requirements_catalogue_.at(prototype_index);
        std::shared_ptr<InteractionTendency> &tendency = tendencies_catalogue_.at(prototype_index);
        return chronicle.CreateInteraction(prototype, requirement, tendency, chance, tick, reasons, participants);
    }
    const std::vector<std::shared_ptr<InteractionRequirement>> &InteractionStore::GetRequirementCatalogue() const
    {
        return requirements_catalogue_;
    }
    const std::vector<std::shared_ptr<InteractionTendency>> &InteractionStore::GetTendencyCatalogue() const
    {
        return tendencies_catalogue_;
    }

    bool InteractionStore::ReadPrototypeJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, std::shared_ptr<InteractionPrototype> &out_prototype)
    {
        TATTLETALE_VERBOSE_PRINT("CREATING PROTOTYPE...");
        out_prototype->ClearValues();

        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype->name, json, name_key_, true, error_preamble))
        {
            return false;
        }

        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype->description, json, description_key_, true, error_preamble))
        {
            return false;
        }

        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype->passive_description, json, passive_description_key_, true, error_preamble))
        {
            return false;
        }
        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(out_prototype->active_description, json, active_description_key_, true, error_preamble))
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
            out_prototype->wealth_effects.push_back(wealth_value);

            nlohmann::json emotion_map;
            if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::object>(emotion_map, emotion_json, i, false, error_preamble))
            {
                return false;
            }
            out_prototype->emotion_effects.push_back(std::vector<float>(static_cast<int>(EmotionType::kLast)));
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
                out_prototype->emotion_effects[i][static_cast<int>(Emotion::StringToEmotionType(key))] = emotion_value;
            }

            nlohmann::json relationship_changes_array;
            if (!ReadJsonValueFromArray<nlohmann::json, nlohmann::detail::value_t::array>(relationship_changes_array, relationship_json, i, false, error_preamble))
            {
                return false;
            }
            size_t index = 0;
            out_prototype->relationship_effects.push_back({});
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
                        TATTLETALE_ERROR_PRINT(true, fmt::format("{} participant index of {} was to big", error_preamble, out_prototype->name));
                        return false;
                    }
                    nlohmann::json changes_json;
                    if (!ReadJsonValueFromDictionary<nlohmann::json, nlohmann::detail::value_t::object>(changes_json, relationship_change_map, changes_key_, false, error_preamble))
                    {
                        return false;
                    }
                    std::vector<float> vector(static_cast<int>(RelationshipType::kLast));
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
                        vector[static_cast<int>(Relationship::StringToRelationshipType(key))] = value;
                    }

                    out_prototype->relationship_effects[i].insert({participant, vector});
                    ++index;
                }
            }
        }

        TATTLETALE_VERBOSE_PRINT(fmt::format("CREATED INTERACTION PROTOTYPE:\n{}\n", out_prototype->ToString()));

        return true;
    }

    bool InteractionStore::ReadRequirementJSON(nlohmann::json json, std::string error_preamble, std::shared_ptr<InteractionRequirement> &out_requirement)
    {
        TATTLETALE_VERBOSE_PRINT("CREATING REQUIREMENTS...");
        out_requirement->ClearValues();
        if (!ReadJsonValueFromDictionary<size_t, nlohmann::detail::value_t::number_unsigned>(out_requirement->participant_count, json, participant_count_key_, true, error_preamble))
        {
            return false;
        }
        std::string context_value = "";
        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(context_value, json, context_key_, false, error_preamble))
        {
            return false;
        }
        out_requirement->context = StringToContextType(context_value);

        std::string goal_type_value = "";
        if (!ReadJsonValueFromDictionary<std::string, nlohmann::detail::value_t::string>(goal_type_value, json, goal_type_key_, false, error_preamble))
        {
            return false;
        }
        out_requirement->goal_type = Goal::StringToGoalType(goal_type_value);

        if (!ReadJsonValueFromDictionary<size_t, nlohmann::detail::value_t::number_unsigned>(out_requirement->day, json, day_key_, false, error_preamble))
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
            out_requirement->emotions[static_cast<int>(Emotion::StringToEmotionType(key))] = emotion_value;
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
            out_requirement->relationship[static_cast<int>(Relationship::StringToRelationshipType(key))] = relationship_value;
        }

        TATTLETALE_VERBOSE_PRINT(fmt::format("CREATED INTERACTION REQUIREMENT:\n{}\n", out_requirement->ToString()));
        return true;
    }

    bool InteractionStore::ReadTendencyJSON(nlohmann::json json, size_t participant_count, std::string error_preamble, std::shared_ptr<InteractionTendency> &out_tendency)
    {
        out_tendency->ClearValues();
        TATTLETALE_VERBOSE_PRINT("CREATING TENDENCY...");

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
            out_tendency->contexts[static_cast<int>(StringToContextType(key))] = context_value;
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
        out_tendency->wealth = wealth_value;

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
            out_tendency->emotions[static_cast<int>(Emotion::StringToEmotionType(key))] = emotion_value;
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
            std::vector<float> relationship_vector(static_cast<int>(RelationshipType::kLast), 0.0f);
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
                relationship_vector[static_cast<int>(Relationship::StringToRelationshipType(key))] = relationship_value;
            }
            out_tendency->relationships.push_back(relationship_vector);
        }

        TATTLETALE_VERBOSE_PRINT(fmt::format("CREATED INTERACTION TENDENCY:\n{}\n", out_tendency->ToString()));
        return true;
    }

    bool InteractionStore::CheckCorrectValueRange(float value, std::string error_preamble)
    {
        float min = -1.0f;
        float max = 1.0f;
        if (value > max)
        {
            TATTLETALE_ERROR_PRINT(true, fmt::format("{}value of {} is bigger than {}", error_preamble, value, max));
            return false;
        }
        if (value < min)
        {
            TATTLETALE_ERROR_PRINT(true, fmt::format("{}value of {} is smaller than {}", error_preamble, value, min));
            return false;
        }
        return true;
    }

} // namespace tattletale