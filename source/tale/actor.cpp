#include "tale/actor.hpp"

#include <iostream>
#include <assert.h>
#include <algorithm>

namespace tale
{
    Actor::Actor(Random &random, const Setting &setting) : random_(random), setting_(setting)
    {
        // TOOD: Create random starting values for everything (including some random relationships)
        name_ = "John Doe";
        size_t tick = 0;
        std::vector<std::weak_ptr<Kernel>> default_reasons;
        goal_ = std::shared_ptr<Goal>(new Goal("goal", tick, default_reasons));
        emotions_ = {
            {
                EmotionType::kHappy,
                std::shared_ptr<Emotion>(new Emotion("happy", tick, default_reasons)),
            },
            {
                EmotionType::kCalm,
                std::shared_ptr<Emotion>(new Emotion("calm", tick, default_reasons)),
            },
            {
                EmotionType::kSatisfied,
                std::shared_ptr<Emotion>(new Emotion("satisfied", tick, default_reasons)),
            },
            {
                EmotionType::kBrave,
                std::shared_ptr<Emotion>(new Emotion("brave", tick, default_reasons)),
            },
            {
                EmotionType::kExtroverted,
                std::shared_ptr<Emotion>(new Emotion("extroverted", tick, default_reasons)),
            },
        };
        traits_.push_back(std::shared_ptr<Trait>(new Trait("trait", tick, default_reasons)));
        resource_ = std::shared_ptr<Resource>(new Resource("resource", tick, default_reasons));
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
} // namespace tale