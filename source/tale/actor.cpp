#include "tale/actor.hpp"

#include <iostream>

namespace tale
{
    Actor::Actor()
    {
        // TOOD: Create random starting values for everything (including some random relationships)
        name_ = "John Doe";
        std::vector<std::weak_ptr<Kernel>> default_reasons;
        goal_ = std::shared_ptr<Goal>(new Goal("goal", default_reasons));
        emotions_ = {
            {
                EmotionType::kHappy,
                std::shared_ptr<Emotion>(new Emotion("happy", default_reasons)),
            },
            {
                EmotionType::kCalm,
                std::shared_ptr<Emotion>(new Emotion("calm", default_reasons)),
            },
            {
                EmotionType::kSatisfied,
                std::shared_ptr<Emotion>(new Emotion("satisfied", default_reasons)),
            },
            {
                EmotionType::kBrave,
                std::shared_ptr<Emotion>(new Emotion("brave", default_reasons)),
            },
            {
                EmotionType::kExtroverted,
                std::shared_ptr<Emotion>(new Emotion("extroverted", default_reasons)),
            },
        };
        traits_.push_back(std::shared_ptr<Trait>(new Trait("trait", default_reasons)));
        resource_ = std::shared_ptr<Resource>(new Resource("resource", default_reasons));
    }
} // namespace tale