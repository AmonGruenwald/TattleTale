#include "tale/actor.hpp"
#include "tale/school.hpp"
#include "tale/school.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>

namespace tale
{
    Actor::Actor(School &school, size_t id, std::string name, size_t tick)
        : random_(school.GetRandom()),
          setting_(school.GetSetting()),
          school_(school),
          interaction_store_(school.GetInteractionStore()),
          id_(id),
          name_(name)
    {
        InitializeRandomWealth(tick, wealth_);
        InitializeRandomEmotions(tick, emotions_);
        InitializeRandomRelationships(tick, relationships_);
        InitializeRandomGoal(tick, goal_);
        InitializeRandomTraits(tick, traits_);
        enrolled_courses_id_ = std::vector<int>(setting_.slot_count_per_week(), -1);
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
        std::vector<std::shared_ptr<Kernel>> reasons;
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
            std::shared_ptr<Kernel> reason(nullptr);
            float chance = CalculateTendencyChance(tendency, context, group_size_ratio, reason);
            if (chance == 0.0f)
            {
                ++zero_count;
            }
            chances.push_back(chance);
            reasons.push_back(reason);
        }

        size_t index = random_.PickIndex(chances, (zero_count == chances.size()));
        out_reasons.push_back(reasons[index]);
        size_t interaction_index = possible_interaction_indices[index];

        out_participants.push_back(weak_from_this());
        std::vector<float> participant_chances;
        const Requirement &requirement = requirements[interaction_index];
        const Tendency &tendency = tendencies[interaction_index];
        for (size_t i = 1; i < requirement.participant_count; ++i)
        {
            for (auto &actor : actor_group)
            {
                // TODO: check if actor is even allowed per requirement
                // TODO: calculate chances per participant
                participant_chances.push_back(1.0f);
            }
            size_t participant_index = random_.PickIndex(participant_chances);
            out_participants.push_back(actor_group[participant_index]);
        }

        return interaction_index;
    }
    bool Actor::CheckRequirements(const Requirement &requirement, const std::vector<std::weak_ptr<Actor>> &actor_group, ContextType context) const
    {
        // TODO: check for other requirements eg. participant count
        if (requirement.context != ContextType::kNone && requirement.context == context)
        {
            return false;
        }
        if (requirement.participant_count > actor_group.size())
        {
            return false;
        }
        return true;
    }
    float Actor::CalculateTendencyChance(const Tendency &tendency, const ContextType &context, const float &group_size_ratio, std::shared_ptr<Kernel> &out_reason)
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

        current_chance_increase = tendency.wealth * wealth_->GetValue();
        chance += current_chance_increase;
        ++chance_parts;
        if (current_chance_increase > highest_chance_increase)
        {
            highest_chance_increase = current_chance_increase;
            out_reason = wealth_;
        }

        for (auto &[type, value] : tendency.emotions)
        {
            current_chance_increase = (value * emotions_[type]->GetValue());
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

    void Actor::ApplyWealthChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, float value)
    {
        float previous_value = wealth_->GetValue();
        float new_value = previous_value + value;
        // TODO: register new resource
        wealth_ = std::shared_ptr<Resource>(new Resource("wealth", tick, reasons, new_value));
    }
    void Actor::ApplyEmotionChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, EmotionType type, float value)
    {
        float previous_value = emotions_[type]->GetValue();
        float new_value = previous_value + value;
        // TODO: register new emotion
        emotions_[type] = std::shared_ptr<Emotion>(new Emotion(type, tick, reasons, new_value));
    }
    void Actor::ApplyRelationshipChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, size_t actor_id, RelationshipType type, float value)
    {
        float previous_value = 0;
        if (relationships_.count(actor_id))
        {
            if (relationships_.at(actor_id).count(type))
            {
                previous_value = relationships_.at(actor_id).at(type)->GetValue();
            }
        }
        float new_value = previous_value + value;
        // TODO: register new emotion
        relationships_[actor_id][type] = std::shared_ptr<Relationship>(new Relationship(type, tick, reasons, new_value));
    }
    std::string Actor::GetWealthDescriptionString()
    {
        return "WEALTH:\n\t" + wealth_->ToString();
    }
    std::string Actor::GetEmotionsDescriptionString()
    {
        std::string emotion_string = "EMOTIONS:";
        for (auto &[type, emotion] : emotions_)
        {
            emotion_string += "\n\t" + emotion->ToString();
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
                relationship_string += "\n\t\t" + relationship->ToString();
            }
        }
        return relationship_string;
    }
    std::string Actor::GetGoalDescriptionString()
    {
        return "GOALS:\n\t" + goal_->ToString();
    }
    std::string Actor::GetTraitsDescriptionString()
    {
        std::string trait_string = "TRAITS:";
        for (auto &trait : traits_)
        {
            trait_string += "\n\t" + trait->ToString();
        }
        return trait_string;
    }
    void Actor::InitializeRandomWealth(size_t tick, std::shared_ptr<Resource> &out_wealth)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_wealth = std::shared_ptr<Resource>(new Resource("wealth", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f)));
    }
    void Actor::InitializeRandomEmotions(size_t tick, std::map<EmotionType, std::shared_ptr<Emotion>> &out_emotions)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_emotions =
            {
                {
                    EmotionType::kHappy,
                    std::shared_ptr<Emotion>(new Emotion(EmotionType::kHappy, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kCalm,
                    std::shared_ptr<Emotion>(new Emotion(EmotionType::kCalm, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kSatisfied,
                    std::shared_ptr<Emotion>(new Emotion(EmotionType::kSatisfied, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kBrave,
                    std::shared_ptr<Emotion>(new Emotion(EmotionType::kBrave, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kExtroverted,
                    std::shared_ptr<Emotion>(new Emotion(EmotionType::kExtroverted, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
            };
    }
    void Actor::InitializeRandomRelationships(size_t tick, std::map<size_t, std::map<RelationshipType, std::shared_ptr<Relationship>>> &out_relationships)
    {
        out_relationships.clear();
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        uint32_t relationship_count = random_.GetUInt(setting_.min_start_relationships_count(), setting_.max_start_relationships_count());
        for (size_t i = 0; i < relationship_count; ++i)
        {
            uint32_t other_actor_id = random_.GetUInt(0, setting_.actor_count - 1);
            while (other_actor_id == id_ || ActorInRelationshipMap(other_actor_id, out_relationships))
            {
                ++other_actor_id;
                other_actor_id %= setting_.actor_count;
            }
            out_relationships.insert(
                {other_actor_id,
                 {
                     {
                         RelationshipType::kLove,
                         std::shared_ptr<Relationship>(new Relationship(RelationshipType::kLove, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                     },
                     {
                         RelationshipType::kAttraction,
                         std::shared_ptr<Relationship>(new Relationship(RelationshipType::kAttraction, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                     },
                     {
                         RelationshipType::kFriendship,
                         std::shared_ptr<Relationship>(new Relationship(RelationshipType::kFriendship, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                     },
                     {
                         RelationshipType::kAnger,
                         std::shared_ptr<Relationship>(new Relationship(RelationshipType::kAnger, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                     },
                     {
                         RelationshipType::kProtective,
                         std::shared_ptr<Relationship>(new Relationship(RelationshipType::kProtective, tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                     },
                 }});
        }
    }
    void Actor::InitializeRandomGoal(size_t tick, std::shared_ptr<Goal> &out_goals)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_goals = std::shared_ptr<Goal>(new Goal("goal", tick, no_reasons));
    }
    void Actor::InitializeRandomTraits(size_t tick, std::vector<std::shared_ptr<Trait>> &out_traits)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_traits = {std::shared_ptr<Trait>(new Trait("trait", tick, no_reasons))};
    }
    bool Actor::ActorInRelationshipMap(size_t actor, const std::map<size_t, std::map<RelationshipType, std::shared_ptr<Relationship>>> &relationships) const
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