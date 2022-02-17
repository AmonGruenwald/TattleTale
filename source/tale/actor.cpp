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
        InitializeRandomResource(tick, resource_);
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

    std::string Actor::ChooseInteraction(const std::vector<std::weak_ptr<Actor>> &course_group, std::vector<std::weak_ptr<Kernel>> &out_reasons, std::vector<std::weak_ptr<Actor>> &out_participants)
    {
        out_participants.push_back(weak_from_this());
        // TODO: this just adds the first other actor to the participants
        if (course_group.at(0).lock().get() != this)
        {
            out_participants.push_back(course_group.at(0));
        }
        else
        {
            // TODO: this should never happen when we choose correctly but with the current status this could crash the application
            if (course_group.size() > 1)
            {
                out_participants.push_back(course_group.at(1));
            }
            else
            {
                out_participants.push_back(course_group.at(0));
            }
        }
        // TODO: add logic for choosing an interaction and fill in reasons correctly

        return interaction_store_.GetRandomInteractionName();
    }

    void Actor::ApplyResourceChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, float value)
    {
        float previous_value = resource_->GetValue();
        float new_value = previous_value + value;
        // TODO: register new resource
        resource_ = std::shared_ptr<Resource>(new Resource("resource", tick, reasons, new_value));
    }
    void Actor::ApplyEmotionChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, EmotionType type, float value)
    {
        float previous_value = emotions_[type]->GetValue();
        float new_value = previous_value + value;
        // TODO: register new emotion
        emotions_[type] = std::shared_ptr<Emotion>(new Emotion(Emotion::EmotionTypeToString(type), tick, reasons, new_value));
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
        relationships_[actor_id][type] = std::shared_ptr<Relationship>(new Relationship(Relationship::RelationshipTypeToString(type), tick, reasons, new_value));
    }

    void Actor::InitializeRandomResource(size_t tick, std::shared_ptr<Resource> &out_resource)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_resource = std::shared_ptr<Resource>(new Resource("resource", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f)));
    }
    void Actor::InitializeRandomEmotions(size_t tick, std::map<EmotionType, std::shared_ptr<Emotion>> &out_emotions)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_emotions =
            {
                {
                    EmotionType::kHappy,
                    std::shared_ptr<Emotion>(new Emotion("happy", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kCalm,
                    std::shared_ptr<Emotion>(new Emotion("calm", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kSatisfied,
                    std::shared_ptr<Emotion>(new Emotion("satisfied", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kBrave,
                    std::shared_ptr<Emotion>(new Emotion("brave", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
                {
                    EmotionType::kExtroverted,
                    std::shared_ptr<Emotion>(new Emotion("extroverted", tick, no_reasons, random_.GetFloat(-1.0f, 1.0f))),
                },
            };
    }
    void Actor::InitializeRandomRelationships(size_t tick, std::map<size_t, std::map<RelationshipType, std::shared_ptr<Relationship>>> &out_relationships)
    {
        std::vector<std::weak_ptr<Kernel>> no_reasons;
        out_relationships = std::map<size_t, std::map<RelationshipType, std::shared_ptr<Relationship>>>();
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
} // namespace tale