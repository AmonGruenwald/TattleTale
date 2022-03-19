#include "shared/actor.hpp"
#include "tale/school.hpp"
#include "tale/school.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include <fmt/core.h>

namespace tattletale
{
    Actor::Actor(School &school, size_t id, std::string first_name, std::string last_name)
        : random_(school.GetRandom()),
          setting_(school.GetSetting()),
          chronicle_(school.GetChronicle()),
          school_(school),
          interaction_store_(school.GetInteractionStore()),
          id_(id),
          first_name_(first_name),
          last_name_(last_name),
          name_(first_name + " " + last_name)
    {
        enrolled_courses_id_ = std::vector<int>(setting_.slot_count_per_week(), -1);
    }

    void Actor::SetupRandomValues(size_t tick)
    {
        InitializeRandomWealth(tick);
        InitializeRandomEmotions(tick);
        InitializeRandomRelationships(tick);
        InitializeRandomGoal(tick);
        InitializeRandomTraits(tick);
    }

    bool Actor::IsEnrolledInCourse(size_t course_id) const
    {
        return std::count(enrolled_courses_id_.begin(), enrolled_courses_id_.end(), course_id);
    }
    void Actor::EnrollInCourse(size_t course_id, uint32_t slot)
    {
        // TODO: actual error handling
        TATTLETALE_ERROR_PRINT(enrolled_courses_id_[slot] == -1, fmt::format("{} already enrolled in a course during slot {}", name_, slot));
        TATTLETALE_ERROR_PRINT(filled_slots_count_ < setting_.slot_count_per_week(), fmt::format("{} already filled their whole schedule", name_))
        ++filled_slots_count_;
        enrolled_courses_id_[slot] = course_id;
    }
    void Actor::EjectFromCourse(size_t course_id, uint32_t slot)
    {
        // TODO: actual error handling
        TATTLETALE_ERROR_PRINT(enrolled_courses_id_[slot] == course_id, fmt::format("Actor is not enrolled in course with id {} during slot {}", course_id, slot));
        --filled_slots_count_;
        enrolled_courses_id_[slot] = -1;
    }
    size_t Actor::GetFilledSlotsCount() const
    {
        return filled_slots_count_;
    }
    bool Actor::AllSlotsFilled() const
    {
        TATTLETALE_ERROR_PRINT(filled_slots_count_ <= setting_.slot_count_per_week(), fmt::format("{} is enrolled in too may courses", name_));
        return filled_slots_count_ == setting_.slot_count_per_week();
    }
    bool Actor::SlotsEmpty(const std::vector<uint32_t> &slots) const
    {
        for (auto &slot : slots)
        {
            if (enrolled_courses_id_[slot] != -1)
            {
                return false;
            }
        }
        return true;
    }
    bool Actor::SlotEmpty(size_t slot) const
    {
        return (enrolled_courses_id_[slot] == -1);
    }

    int Actor::ChooseInteraction(const std::vector<Actor *> &actor_group, ContextType context, std::vector<Kernel *> &out_reasons, std::vector<Actor *> &out_participants, float &out_chance)
    {
        const std::vector<std::shared_ptr<InteractionRequirement>> &requirements = interaction_store_.GetRequirementCatalogue();
        std::vector<size_t> possible_interaction_indices;
        for (size_t i = 0; i < requirements.size(); ++i)
        {
            if (CheckRequirements(*requirements[i], actor_group, context))
            {
                possible_interaction_indices.push_back(i);
            }
        }
        if (possible_interaction_indices.size() == 0)
        {
            return -1;
        }

        const std::vector<std::shared_ptr<InteractionTendency>> &tendencies = interaction_store_.GetTendencyCatalogue();
        std::vector<float> chances;
        chances.reserve(possible_interaction_indices.size());
        std::vector<Kernel *> tendency_reasons;
        std::vector<Kernel *> goal_reasons;
        tendency_reasons.reserve(possible_interaction_indices.size());
        goal_reasons.reserve(possible_interaction_indices.size());
        uint32_t zero_count = 0;
        for (auto &i : possible_interaction_indices)
        {
            std::shared_ptr<InteractionTendency> tendency = tendencies[i];
            Kernel *tendency_reason = nullptr;
            float chance = CalculateTendencyChance(*tendency, context, tendency_reason);
            bool goal_had_effect = false;
            float modified_chance = ApplyGoalChanceModification(chance, i, goal_had_effect);
            if (goal_had_effect)
            {
                goal_reasons.push_back(goal_);
            }
            else
            {
                goal_reasons.push_back(nullptr);
            }
            if (chance == 0.0f)
            {
                ++zero_count;
            }
            chances.push_back(chance);
            tendency_reasons.push_back(tendency_reason);
        }

        size_t index = random_.PickIndex(chances, (zero_count == chances.size()));
        if (tendency_reasons[index])
        {
            out_reasons.push_back(tendency_reasons[index]);
        }
        if (goal_reasons[index])
        {
            out_reasons.push_back(goal_reasons[index]);
        }
        out_chance = chances[index];
        size_t interaction_index = possible_interaction_indices[index];
        if (interaction_index = 1 && first_name_ == "Robin")
        {
            int aasdlf = 0;
        }
        out_participants.push_back(this);
        const std::shared_ptr<InteractionRequirement> &requirement = requirements[interaction_index];
        const std::shared_ptr<InteractionTendency> &tendency = tendencies[interaction_index];
        for (size_t i = 1; i < requirement->participant_count; ++i)
        {
            uint32_t participant_zero_count = 0;
            std::vector<float> participant_chances;

            std::vector<Kernel *> participant_reasons;
            for (auto &actor : actor_group)
            {
                Kernel *reason = nullptr;
                size_t id = actor->id_;
                if (relationships_.count(id))
                {
                    std::map<RelationshipType, Relationship *> relationship_map = relationships_[id];

                    float chance = 0.0f;
                    float highest_chance_increase = 0.0f;
                    float current_chance_increase = 0.0f;
                    uint16_t chance_parts = 0;

                    bool requirement_failed = false;
                    for (auto &[type, relationship] : relationship_map)
                    {
                        current_chance_increase = relationship->GetValue() * tendency->relationships[i - 1].at(type);
                        chance += current_chance_increase;
                        ++chance_parts;

                        if (current_chance_increase > highest_chance_increase)
                        {
                            highest_chance_increase = current_chance_increase;
                            reason = relationship;
                        }
                        if (requirement->relationship.at(type) < 0)
                        {
                            if (relationship->GetValue() > requirement->relationship.at(type))
                            {
                                requirement_failed = true;
                            }
                        }
                        else if (requirement->relationship.at(type) > 0)
                        {
                            if (relationship->GetValue() < requirement->relationship.at(type))
                            {
                                requirement_failed = true;
                            }
                        }
                    }
                    if (requirement_failed)
                    {
                        chance = 0.0f;
                    }
                    chance += static_cast<float>(chance_parts);
                    chance /= static_cast<float>(chance_parts * 2);
                    if (chance == 0.0f)
                    {
                        ++participant_zero_count;
                    }
                    participant_chances.push_back(chance);
                }
                else
                {
                    if (requirement->HasEmotionalRequirement())
                    {
                        participant_chances.push_back(0.0f);
                        ++participant_zero_count;
                    }
                    else
                    {
                        participant_chances.push_back(0.5f);
                    }
                }
                participant_reasons.push_back(reason);
            }
            size_t participant_index = random_.PickIndex(participant_chances, (participant_zero_count == participant_chances.size()));
            // TODO: I think it is possible for actors to be assigned multiple times as targets for an interaction
            while (actor_group[participant_index]->id_ == id_)
            {
                participant_index += 1;
                participant_index %= actor_group.size();
            }
            out_participants.push_back(actor_group[participant_index]);
            if (participant_reasons[participant_index])
            {
                out_reasons.push_back(participant_reasons[participant_index]);
            }
        }
        return interaction_index;
    }
    bool Actor::CheckRequirements(const InteractionRequirement &requirement, const std::vector<Actor *> &actor_group, ContextType context) const
    {
        // TODO: check for other requirements eg. participant count
        if (requirement.context != ContextType::kNone && requirement.context != context)
        {
            return false;
        }
        if (requirement.participant_count > actor_group.size())
        {
            return false;
        }
        if (requirement.goal_type != GoalType::kNone && requirement.goal_type != goal_->type_)
        {
            return false;
        }
        if (requirement.day > school_.GetCurrentDay())
        {
            return false;
        }
        for (auto &[key, value] : requirement.emotions)
        {
            if (value < 0)
            {
                if (emotions_.at(key)->GetValue() > value)
                {
                    return false;
                }
            }
            else if (value > 0)
            {
                if (emotions_.at(key)->GetValue() < value)
                {
                    return false;
                }
            }
        }
        bool match_found = true;
        for (auto &[actor_id, relationship] : relationships_)
        {
            match_found = true;
            for (auto &[key, value] : requirement.relationship)
            {
                if (value < 0)
                {
                    if (relationship.at(key)->GetValue() > value)
                    {
                        match_found = false;
                    }
                }
                else if (value > 0)
                {
                    if (relationship.at(key)->GetValue() < value)
                    {
                        match_found = false;
                    }
                }
            }
            if (match_found)
            {
                break;
            }
        }
        if (!match_found)
        {
            return false;
        }
        return true;
    }
    float Actor::CalculateTendencyChance(const InteractionTendency &tendency, const ContextType &context, Kernel *&out_reason)
    {
        // TODO: reasons only track positive chance, they do not use reasons why we did not pick other interactions
        // TODO: reasons also will never include groupsize or context
        float chance = 0.0f;
        float highest_chance_influence = 0.0f;
        float current_chance_increase = 0.0f;
        uint16_t chance_parts = 0;

        for (auto &[type, value] : tendency.contexts)
        {
            // TODO: rethink wether this makes sense
            current_chance_increase = (value * (context == type ? 1.0f : -1.0f));
            chance += current_chance_increase;
            ++chance_parts;
        }

        current_chance_increase = tendency.wealth * wealth_->GetValue();
        chance += current_chance_increase;
        ++chance_parts;
        if (current_chance_increase > highest_chance_influence)
        {
            highest_chance_influence = current_chance_increase;
            out_reason = wealth_;
        }

        for (auto &[type, value] : tendency.emotions)
        {
            current_chance_increase = (value * emotions_[type]->GetValue());
            chance += current_chance_increase;
            ++chance_parts;
            if (current_chance_increase > highest_chance_influence)
            {
                highest_chance_influence = current_chance_increase;
                out_reason = emotions_[type];
            }
        }

        chance += static_cast<float>(chance_parts);
        chance /= static_cast<float>(chance_parts * 2);
        return chance;
    }
    float Actor::ApplyGoalChanceModification(float original_chance, size_t interaction_index, bool &out_had_positive_effect)
    {
        float relevant_effect = 0;
        const auto &effects = interaction_store_.GetRelationshipEffects(interaction_index);
        switch (goal_->type_)
        {
        case GoalType::kNone:
            TATTLETALE_ERROR_PRINT(true, "Trying to apply invalid goal type");
            break;
        case GoalType::kLast:
            TATTLETALE_ERROR_PRINT(true, "Trying to apply invalid goal type");
            break;
        case GoalType::kWealth:
            relevant_effect = interaction_store_.GetWealthEffects(interaction_index)[0];
            break;
        case GoalType::kAcceptance:
            // skip first as we want the values of other people to this actor
            for (size_t i = 1; i < effects.size(); ++i)
            {
                if (effects[i].count(0))
                {
                    relevant_effect += effects[i].at(0).at(RelationshipType::kFriendship);
                }
            }
            break;
        case GoalType::kRelationship:
            // skip first as we want the values of other people to this actor
            for (size_t i = 1; i < effects.size(); ++i)
            {
                // do both sides of the relationship change
                if (effects[i].count(0) && effects[0].count(i))
                {
                    relevant_effect += effects[i].at(0).at(RelationshipType::kLove);
                    relevant_effect += effects[0].at(1).at(RelationshipType::kLove);
                }
            }
            break;
        case GoalType::kHedonism:
            relevant_effect = interaction_store_.GetEmotionEffects(interaction_index)[0].at(EmotionType::kSatisfied);
            break;
        case GoalType::kPower:
            const auto &effects = interaction_store_.GetRelationshipEffects(interaction_index);
            // skip first as we want the values of other people to this actor
            for (const auto &[other_actor, effect] : effects[0])
            {

                // using minus here because the actor wants a negative value
                relevant_effect -= effect.at(RelationshipType::kProtective);
            }
            break;
        }
        relevant_effect = std::clamp(relevant_effect, -1.0f, 1.0f);
        out_had_positive_effect = relevant_effect > 0;
        // seet https://www.desmos.com/calculator/ojqnu2ni4c
        return pow(original_chance, (1.0f - relevant_effect));
    }

    void Actor::ApplyWealthChange(const std::vector<Kernel *> &reasons, size_t tick, float value)
    {
        if (value == 0)
        {
            return;
        }
        float previous_value = wealth_->GetValue();
        // TODO: think about handling this cleaner
        float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
        std::vector<Kernel *> all_reasons(reasons);
        all_reasons.push_back(wealth_);
        wealth_ = chronicle_.CreateResource("wealth", "wealthy", "poor", tick, this, all_reasons, new_value);
    }
    void Actor::ApplyEmotionChange(const std::vector<Kernel *> &reasons, size_t tick, EmotionType type, float value)
    {
        if (value == 0)
        {
            return;
        }
        float previous_value = emotions_[type]->GetValue();
        // TODO: think about handling this cleaner
        float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
        std::vector<Kernel *> all_reasons(reasons);
        all_reasons.push_back(emotions_[type]);
        emotions_[type] = chronicle_.CreateEmotion(type, tick, this, all_reasons, new_value);
    }
    void Actor::ApplyRelationshipChange(const std::vector<Kernel *> &reasons, size_t tick, size_t actor_id, std::map<RelationshipType, float> change)
    {
        bool all_zero = true;
        for (auto &[type, value] : change)
        {
            if (value != 0)
            {
                all_zero = false;
            }
        }
        if (all_zero)
        {
            return;
        }
        bool already_known = relationships_.count(actor_id);
        auto other_actor = school_.GetActor(actor_id);
        if (!already_known)
        {
            known_actors_.push_back(school_.GetActor(actor_id));
        }
        std::vector<Kernel *> all_reasons;
        for (auto &[type, value] : change)
        {
            all_reasons.clear();
            all_reasons.insert(all_reasons.end(), reasons.begin(), reasons.end());
            float previous_value = 0;
            if (already_known)
            {
                if (value == 0)
                {
                    continue;
                }
                previous_value = relationships_.at(actor_id).at(type)->GetValue();
                all_reasons.push_back(relationships_.at(actor_id).at(type));
            }
            // TODO: think about handling this cleaner
            float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
            relationships_[actor_id][type] = chronicle_.CreateRelationship(type, tick, this, other_actor, all_reasons, new_value);
        }
        UpdateKnownActors();
    }

    std::string Actor::GetDetailedDescriptionString() const
    {
        std::string detailed_actor_description = fmt::format("{}:", name_);
        detailed_actor_description += fmt::format("\n\t{:o}", *wealth_);
        for (auto &[type, emotion] : emotions_)
        {
            detailed_actor_description += fmt::format("\n\t{:o}", *emotion);
        }
        for (auto &[actor_index, map] : relationships_)
        {
            // TODO: this will only work after all actors have been created, which is wonky
            Actor *other_actor = school_.GetActor(actor_index);
            detailed_actor_description += fmt::format("\n\tWith #{} {}:", other_actor->id_, *other_actor);
            for (auto &[type, relationship] : map)
            {
                detailed_actor_description += fmt::format("\n\t\t{:o}", *relationship);
            }
        }
        detailed_actor_description += fmt::format("\n\t{:o}", *goal_);
        for (auto &trait : traits_)
        {
            detailed_actor_description += fmt::format("\n\t{:o}", *trait);
        }
        return detailed_actor_description;
    }

    const std::vector<Actor *> &Actor::GetAllKnownActors() const
    {
        return known_actors_;
    }
    std::vector<Actor *> Actor::GetFreetimeActorGroup() const
    {
        if (setting_.freetime_actor_count >= known_actors_.size())
        {
            return known_actors_;
        }
        std::vector<Actor *> freetime_actors = {known_actors_.begin(), known_actors_.begin() + setting_.freetime_actor_count};
        return freetime_actors;
    }
    void Actor::InitializeRandomWealth(size_t tick)
    {
        std::vector<Kernel *> no_reasons;
        wealth_ = chronicle_.CreateResource("wealth", "wealthy", "poor", tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f));
    }
    void Actor::InitializeRandomEmotions(size_t tick)
    {
        std::vector<Kernel *> no_reasons;
        emotions_ =
            {
                {
                    EmotionType::kHappy,
                    chronicle_.CreateEmotion(EmotionType::kHappy, tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kCalm,
                    chronicle_.CreateEmotion(EmotionType::kCalm, tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kSatisfied,
                    chronicle_.CreateEmotion(EmotionType::kSatisfied, tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kBrave,
                    chronicle_.CreateEmotion(EmotionType::kBrave, tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kExtroverted,
                    chronicle_.CreateEmotion(EmotionType::kExtroverted, tick, this, no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
            };
    }
    void Actor::InitializeRandomRelationships(size_t tick)
    {
        std::vector<Kernel *> no_reasons;
        uint32_t relationship_count = random_.GetUInt(setting_.min_start_relationships_count(), setting_.max_start_relationships_count());
        for (size_t i = relationships_.size(); i < relationship_count; ++i)
        {
            uint32_t other_actor_id = random_.GetUInt(0, setting_.actor_count - 1);
            auto other_actor = school_.GetActor(other_actor_id);
            size_t tries = 0;
            while ((other_actor_id == id_ || HasRelationshipWith(other_actor_id) || other_actor->GetAllKnownActors().size() > setting_.desired_max_start_relationships_count) &&
                   tries < setting_.actor_count)
            {
                ++tries;
                ++other_actor_id;
                other_actor_id %= setting_.actor_count;
            }
            if (tries == setting_.actor_count)
            {
                return;
            }
            std::map<RelationshipType, Relationship *> relationship =
                {
                    {RelationshipType::kLove, chronicle_.CreateRelationship(RelationshipType::kLove, tick, this, other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kAttraction, chronicle_.CreateRelationship(RelationshipType::kAttraction, tick, this, other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kFriendship, chronicle_.CreateRelationship(RelationshipType::kFriendship, tick, this, other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kAnger, chronicle_.CreateRelationship(RelationshipType::kAnger, tick, this, other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kProtective, chronicle_.CreateRelationship(RelationshipType::kProtective, tick, this, other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))}};
            InsertNewRelationship(other_actor, relationship);
            std::map<RelationshipType, Relationship *> other_relationship =
                {
                    {RelationshipType::kLove, chronicle_.CreateRelationship(RelationshipType::kLove, tick, other_actor, this, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kAttraction, chronicle_.CreateRelationship(RelationshipType::kAttraction, tick, other_actor, this, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kFriendship, chronicle_.CreateRelationship(RelationshipType::kFriendship, tick, other_actor, this, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kAnger, chronicle_.CreateRelationship(RelationshipType::kAnger, tick, other_actor, this, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                    {RelationshipType::kProtective, chronicle_.CreateRelationship(RelationshipType::kProtective, tick, other_actor, this, no_reasons, random_.GetFloat(-1.0f, 1.0f))}};
            other_actor->InsertNewRelationship(this, relationship);
        }
    }

    void Actor::InsertNewRelationship(Actor *other_actor, std::map<RelationshipType, Relationship *> relationship)
    {
        relationships_.insert({other_actor->id_, relationship});
        known_actors_.push_back(other_actor);
        UpdateKnownActors();
    }
    void Actor::InitializeRandomGoal(size_t tick)
    {
        std::vector<Kernel *> no_reasons;
        goal_ = chronicle_.CreateGoal(Goal::GetRandomGoalType(random_), tick, this, no_reasons);
    }
    void Actor::InitializeRandomTraits(size_t tick)
    {
        std::vector<Kernel *> no_reasons;
        traits_ = {chronicle_.CreateTrait("trait", tick, this, no_reasons)};
    }

    void Actor::UpdateKnownActors()
    {
        std::sort(known_actors_.begin(), known_actors_.end(), [this](const Actor *lhs, const Actor *rhs)
                  {
                      float lhs_value = CalculateRelationshipValue(lhs->id_);
                      float rhs_value = CalculateRelationshipValue(rhs->id_);
                      return lhs_value > rhs_value; });
    }

    float Actor::CalculateRelationshipValue(size_t actor_id) const
    {
        float value = 0;
        for (const auto &[type, relationship] : relationships_.at(actor_id))
        {
            value += abs(relationship->GetValue());
        }
        return value;
    }

    bool Actor::HasRelationshipWith(size_t actor_id) const
    {
        for (auto &[key, value] : relationships_)
        {
            if (key == actor_id)
            {
                return true;
            }
        }
        return false;
    }
} // namespace tattletale