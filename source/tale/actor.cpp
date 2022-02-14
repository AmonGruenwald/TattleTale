#include "tale/actor.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>

namespace tale
{
    Actor::Actor(Random &random, const Setting &setting, InteractionStore &interaction_store) : random_(random), setting_(setting), interaction_store_(interaction_store)
    {
        // TOOD: Create random starting values for everything (including some random relationships) and register those
        name_ = "John Doe";
        size_t tick = 0;
        std::vector<std::weak_ptr<Kernel>> default_reasons;
        goal_ = std::shared_ptr<Goal>(new Goal("goal", tick, default_reasons));
        emotions_ = {
            {
                EmotionType::kHappy,
                std::shared_ptr<Emotion>(new Emotion("happy", tick, default_reasons, 1)),
            },
            {
                EmotionType::kCalm,
                std::shared_ptr<Emotion>(new Emotion("calm", tick, default_reasons, 1)),
            },
            {
                EmotionType::kSatisfied,
                std::shared_ptr<Emotion>(new Emotion("satisfied", tick, default_reasons, 1)),
            },
            {
                EmotionType::kBrave,
                std::shared_ptr<Emotion>(new Emotion("brave", tick, default_reasons, 1)),
            },
            {
                EmotionType::kExtroverted,
                std::shared_ptr<Emotion>(new Emotion("extroverted", tick, default_reasons, 1)),
            },
        };
        traits_.push_back(std::shared_ptr<Trait>(new Trait("trait", tick, default_reasons)));
        resource_ = std::shared_ptr<Resource>(new Resource("resource", tick, default_reasons, 1));
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

    std::string Actor::ChooseInteraction(const std::vector<std::weak_ptr<Actor>> &course_group, std::vector<std::weak_ptr<Kernel>> out_reasons, std::vector<std::weak_ptr<Actor>> out_participants)
    {
        out_participants.push_back(weak_from_this());
        // TODO: this just adds the first other actor to the participants
        if (course_group.at(0).lock().get() != this)
        {
            out_participants.push_back(course_group.at(0));
        }
        else
        {
            out_participants.push_back(course_group.at(1));
        }
        // TODO: add logic for choosing an interaction and fill in reasons correctly

        return interaction_store_.GetRandomInteractionName();
    }

} // namespace tale