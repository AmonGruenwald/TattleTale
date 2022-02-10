#include "tale/actor.hpp"

#include <iostream>
#include <assert.h>
#include <algorithm>

namespace tale
{
    Actor::Actor()
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
    }

    bool Actor::IsEnrolledInCourse(size_t course_id)
    {
        return std::count(enrolled_courses_id_.begin(), enrolled_courses_id_.end(), course_id);
    }

    void Actor::EnrollInCourse(size_t course_id)
    {
        assert(!IsEnrolledInCourse(course_id));
        enrolled_courses_id_.push_back(course_id);
    }
} // namespace tale