#include "tale/actor.hpp"
#include "tale/school.hpp"
#include "tale/school.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>

namespace tale
{
    Actor::Actor(School &school, size_t id, std::string name)
        : random_(school.GetRandom()),
          setting_(school.GetSetting()),
          chronicle_(school.GetChronicle()),
          school_(school),
          interaction_store_(school.GetInteractionStore()),
          id_(id),
          name_(name)
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
        assert(enrolled_courses_id_[slot] == -1);
        assert(filled_slots_count_ < setting_.slot_count_per_week()); // enrolled in too many courses
        ++filled_slots_count_;
        enrolled_courses_id_[slot] = course_id;
    }

    size_t Actor::GetFilledSlotsCount() const
    {
        return filled_slots_count_;
    }

    bool Actor::AllSlotsFilled() const
    {
        assert(filled_slots_count_ <= setting_.slot_count_per_week()); // enrolled in too many courses
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

    int Actor::ChooseInteraction(const std::vector<std::weak_ptr<Actor>> &actor_group, ContextType context, std::vector<std::weak_ptr<Kernel>> &out_reasons, std::vector<std::weak_ptr<Actor>> &out_participants)
    {
        const std::vector<Requirement> &requirements = interaction_store_.GetRequirementCatalogue();
        std::vector<size_t> possible_interaction_indices;
        for (size_t i = 0; i < requirements.size(); ++i)
        {
            if (CheckRequirements(requirements[i], actor_group, context))
            {
                possible_interaction_indices.push_back(i);
            }
        }
        if (possible_interaction_indices.size() == 0)
        {
            return -1;
        }

        const std::vector<Tendency> &tendencies = interaction_store_.GetTendencyCatalogue();
        std::vector<float> chances;
        chances.reserve(possible_interaction_indices.size());
        std::vector<std::weak_ptr<Kernel>> reasons;
        reasons.reserve(possible_interaction_indices.size());
        uint32_t zero_count = 0;
        for (auto &i : possible_interaction_indices)
        {
            const Tendency &tendency = tendencies[i];
            // TODO: rethink if this makes sense
            float group_size_ratio = (context == ContextType::kCourse
                                          ? static_cast<float>(actor_group.size())
                                          : static_cast<float>(requirements.at(i).participant_count)) /
                                     static_cast<float>(setting_.actors_per_course);
            group_size_ratio = std::clamp(group_size_ratio, 0.0f, 1.0f);
            group_size_ratio *= 2;
            group_size_ratio -= 1.0f;
            std::weak_ptr<Kernel> reason;
            float chance = CalculateTendencyChance(tendency, context, group_size_ratio, reason);
            float modified_chance = ApplyGoalChanceModification(chance, i);
            // TODO: this value is completely random
            if (abs(modified_chance - chance) > 0.25f)
            {
                reason = goal_;
            }
            if (chance == 0.0f)
            {
                ++zero_count;
            }
            chances.push_back(chance);
            reasons.push_back(reason);
        }

        size_t index = random_.PickIndex(chances, (zero_count == chances.size()));
        if (reasons[index].lock())
        {
            out_reasons.push_back(reasons[index]);
        }
        size_t interaction_index = possible_interaction_indices[index];

        out_participants.push_back(weak_from_this());
        std::vector<float> participant_chances;
        uint32_t participant_zero_count = 0;
        const Requirement &requirement = requirements[interaction_index];
        const Tendency &tendency = tendencies[interaction_index];
        for (size_t i = 1; i < requirement.participant_count; ++i)
        {

            std::vector<std::weak_ptr<Kernel>> participant_reasons;
            for (auto &actor : actor_group)
            {
                // TODO: check if actor is even allowed per requirement
                std::weak_ptr<Kernel> reason;
                size_t id = actor.lock()->id_;
                if (relationships_.count(id))
                {
                    std::map<RelationshipType, std::weak_ptr<Relationship>> relationship_map = relationships_[id];

                    float chance = 0.0f;
                    float highest_chance_increase = 0.0f;
                    float current_chance_increase = 0.0f;
                    uint16_t chance_parts = 0;

                    bool requirement_failed = false;
                    for (auto &[type, relationship] : relationship_map)
                    {
                        current_chance_increase = relationship.lock()->GetValue() * tendency.relationships[i - 1].at(type);
                        chance += current_chance_increase;
                        ++chance_parts;

                        if (current_chance_increase > highest_chance_increase)
                        {
                            highest_chance_increase = current_chance_increase;
                            reason = relationship;
                        }
                        if (requirement.relationship.at(type) < 0)
                        {
                            if (relationship.lock()->GetValue() > requirement.relationship.at(type))
                            {
                                requirement_failed = true;
                            }
                        }
                        else if (requirement.relationship.at(type) > 0)
                        {
                            if (relationship.lock()->GetValue() < requirement.relationship.at(type))
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
                    if (requirement.HasEmotionalRequirement())
                    {
                        participant_chances.push_back(0.0f);
                    }
                    else
                    {
                        participant_chances.push_back(0.5f);
                    }
                }
                participant_reasons.push_back(reason);
            }
            size_t participant_index = random_.PickIndex(participant_chances, (participant_zero_count == participant_chances.size()));
            out_participants.push_back(actor_group[participant_index]);
            if (participant_reasons[participant_index].lock())
            {
                out_reasons.push_back(participant_reasons[participant_index]);
            }
        }
        return interaction_index;
    }
    bool Actor::CheckRequirements(const Requirement &requirement, const std::vector<std::weak_ptr<Actor>> &actor_group, ContextType context) const
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
        if (requirement.goal_type != GoalType::kNone && requirement.goal_type != goal_.lock()->type_)
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
                if (emotions_.at(key).lock()->GetValue() > value)
                {
                    return false;
                }
            }
            else if (value > 0)
            {
                if (emotions_.at(key).lock()->GetValue() < value)
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
                    if (relationship.at(key).lock()->GetValue() > value)
                    {
                        match_found = false;
                    }
                }
                else if (value > 0)
                {
                    if (relationship.at(key).lock()->GetValue() < value)
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

    float Actor::CalculateTendencyChance(const Tendency &tendency, const ContextType &context, const float &group_size_ratio, std::weak_ptr<Kernel> &out_reason)
    {
        // TODO: reasons only track positive chance, they do not use reasons why we did not pick other interactions
        // TODO: reasons also will never include groupsize or context
        float chance = 0.0f;
        float highest_chance_increase = 0.0f;
        float current_chance_increase = 0.0f;
        uint16_t chance_parts = 0;

        current_chance_increase = tendency.group_size * group_size_ratio;
        chance += current_chance_increase;
        ++chance_parts;
        for (auto &[type, value] : tendency.contexts)
        {
            // TODO: rethink wether this makes sense
            current_chance_increase = (value * (context == type ? 1.0f : -1.0f));
            chance += current_chance_increase;
            ++chance_parts;
        }

        current_chance_increase = tendency.wealth * wealth_.lock()->GetValue();
        chance += current_chance_increase;
        ++chance_parts;
        if (current_chance_increase > highest_chance_increase)
        {
            highest_chance_increase = current_chance_increase;
            out_reason = wealth_;
        }

        for (auto &[type, value] : tendency.emotions)
        {
            current_chance_increase = (value * emotions_[type].lock()->GetValue());
            chance += current_chance_increase;
            ++chance_parts;
            if (current_chance_increase > highest_chance_increase)
            {
                highest_chance_increase = current_chance_increase;
                out_reason = emotions_[type];
            }
        }

        chance += static_cast<float>(chance_parts);
        chance /= static_cast<float>(chance_parts * 2);
        return chance;
    }

    float Actor::ApplyGoalChanceModification(float original_chance, size_t interaction_index)
    {
        float relevant_effect = 0;
        const auto &effects = interaction_store_.GetRelationshipEffects(interaction_index);
        switch (goal_.lock()->type_)
        {
        case GoalType::kNone:
            assert(false); // invalid goal type was stored
            break;
        case GoalType::kLast:
            assert(false); // invalid goal type was stored
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

        // seet https://www.desmos.com/calculator/ojqnu2ni4c
        return pow(original_chance, (1.0f - relevant_effect));
    }

    void Actor::ApplyWealthChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, float value)
    {
        if (value == 0)
        {
            return;
        }
        float previous_value = wealth_.lock()->GetValue();
        // TODO: think about handling this cleaner
        float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
        std::vector<std::weak_ptr<Kernel>> all_reasons(reasons);
        all_reasons.push_back(wealth_);
        wealth_ = chronicle_.CreateResource("wealth", tick, weak_from_this(), all_reasons, new_value);
    }
    void Actor::ApplyEmotionChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, EmotionType type, float value)
    {
        if (value == 0)
        {
            return;
        }
        float previous_value = emotions_[type].lock()->GetValue();
        // TODO: think about handling this cleaner
        float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
        std::vector<std::weak_ptr<Kernel>> all_reasons(reasons);
        all_reasons.push_back(emotions_[type]);
        emotions_[type] = chronicle_.CreateEmotion(type, tick, weak_from_this(), all_reasons, new_value);
    }
    void Actor::ApplyRelationshipChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, size_t actor_id, RelationshipType type, float value)
    {
        float previous_value = 0;
        std::vector<std::weak_ptr<Kernel>> all_reasons(reasons);
        if (relationships_.count(actor_id))
        {

            if (relationships_.at(actor_id).count(type))
            {
                if (value == 0)
                {
                    return;
                }
                previous_value = relationships_.at(actor_id).at(type).lock()->GetValue();
                all_reasons.push_back(relationships_.at(actor_id).at(type));
            }
        }
        // TODO: think about handling this cleaner
        float new_value = std::clamp(previous_value + value, -1.0f, 1.0f);
        auto other_actor = school_.GetActor(actor_id);
        relationships_[actor_id][type] = chronicle_.CreateRelationship(type, tick, weak_from_this(), other_actor, all_reasons, new_value);
    }
    std::string Actor::GetWealthDescriptionString()
    {
        return "WEALTH:\n\t" + wealth_.lock()->ToString();
    }
    std::string Actor::GetEmotionsDescriptionString()
    {
        std::string emotion_string = "EMOTIONS:";
        for (auto &[type, emotion] : emotions_)
        {
            emotion_string += "\n\t" + emotion.lock()->ToString();
        }
        return emotion_string;
    }
    std::string Actor::GetRelationshipsDescriptionString()
    {
        std::string relationship_string = "RELATIONSHIPS:";
        for (auto &[actor_index, map] : relationships_)
        {
            // TODO: this will only work after all actors have been created, which is wonky
            relationship_string += ("\n\tWith " + school_.GetActor(actor_index).lock()->name_ + " (ID: " + std::to_string(school_.GetActor(actor_index).lock()->id_) + "):");
            for (auto &[type, relationship] : map)
            {
                relationship_string += "\n\t\t" + relationship.lock()->ToString();
            }
        }
        return relationship_string;
    }
    std::string Actor::GetGoalDescriptionString()
    {
        return "GOALS:\n\t" + goal_.lock()->ToString();
    }
    std::string Actor::GetTraitsDescriptionString()
    {
        std::string trait_string = "TRAITS:";
        for (auto &trait : traits_)
        {
            trait_string += "\n\t" + trait.lock()->ToString();
        }
        return trait_string;
    }
    void Actor::InitializeRandomWealth(size_t tick)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        wealth_ = chronicle_.CreateResource("wealth", tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f));
    }
    void Actor::InitializeRandomEmotions(size_t tick)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        emotions_ =
            {
                {
                    EmotionType::kHappy,
                    chronicle_.CreateEmotion(EmotionType::kHappy, tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kCalm,
                    chronicle_.CreateEmotion(EmotionType::kCalm, tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kSatisfied,
                    chronicle_.CreateEmotion(EmotionType::kSatisfied, tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kBrave,
                    chronicle_.CreateEmotion(EmotionType::kBrave, tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
                {
                    EmotionType::kExtroverted,
                    chronicle_.CreateEmotion(EmotionType::kExtroverted, tick, weak_from_this(), no_reasons, random_.GetFloat(-1.0f, 1.0f)),
                },
            };
    }
    void Actor::InitializeRandomRelationships(size_t tick)
    {
        relationships_.clear();
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        uint32_t relationship_count = random_.GetUInt(setting_.min_start_relationships_count(), setting_.max_start_relationships_count());
        for (size_t i = 0; i < relationship_count; ++i)
        {
            uint32_t other_actor_id = random_.GetUInt(0, setting_.actor_count - 1);
            while (other_actor_id == id_ || ActorInRelationshipMap(other_actor_id, relationships_))
            {
                ++other_actor_id;
                other_actor_id %= setting_.actor_count;
            }
            auto other_actor = school_.GetActor(other_actor_id);
            relationships_.insert(
                {other_actor_id,
                 {{RelationshipType::kLove,
                   chronicle_.CreateRelationship(RelationshipType::kLove, tick, weak_from_this(), other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                  {RelationshipType::kAttraction,
                   chronicle_.CreateRelationship(RelationshipType::kAttraction, tick, weak_from_this(), other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                  {RelationshipType::kFriendship,
                   chronicle_.CreateRelationship(RelationshipType::kFriendship, tick, weak_from_this(), other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                  {RelationshipType::kAnger,
                   chronicle_.CreateRelationship(RelationshipType::kAnger, tick, weak_from_this(), other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))},
                  {RelationshipType::kProtective,
                   chronicle_.CreateRelationship(RelationshipType::kProtective, tick, weak_from_this(), other_actor, no_reasons, random_.GetFloat(-1.0f, 1.0f))}}});
        }
    }
    void Actor::InitializeRandomGoal(size_t tick)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        goal_ = chronicle_.CreateGoal(Goal::GetRandomGoalType(random_), tick, no_reasons);
    }
    void Actor::InitializeRandomTraits(size_t tick)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        traits_ = {chronicle_.CreateTrait("trait", tick, no_reasons)};
    }
    bool Actor::ActorInRelationshipMap(size_t actor, const std::map<size_t, std::map<RelationshipType, std::weak_ptr<Relationship>>> &relationships) const
    {
        for (auto &[key, value] : relationships)
        {
            if (key == actor)
            {
                return true;
            }
        }
        return false;
    }
} // namespace tale