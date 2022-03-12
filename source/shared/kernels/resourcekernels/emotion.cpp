#include "shared/kernels/resourcekernels/emotion.hpp"
#include <assert.h>
#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tale
{
    Emotion::Emotion(EmotionType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
        : Resource(EmotionTypeToString(type), id, tick, owner, reasons, value), type_(type){};

    EmotionType Emotion::StringToEmotionType(std::string emotion_string)
    {
        if (emotion_string == "happy")
            return EmotionType::kHappy;
        if (emotion_string == "calm")
            return EmotionType::kCalm;
        if (emotion_string == "satisfied")
            return EmotionType::kSatisfied;
        if (emotion_string == "brave")
            return EmotionType::kBrave;
        if (emotion_string == "extroverted")
            return EmotionType::kExtroverted;
        assert(false); // invalid string was passed
        return EmotionType::kNone;
    }
    std::string Emotion::EmotionTypeToString(EmotionType emotion_type)
    {
        switch (emotion_type)
        {
        case EmotionType::kNone:
            assert(false); // invalid enum was passed
            return "none";
            break;
        case EmotionType::kLast:
            assert(false); // invalid enum was passed
            return "last";
            break;
        case EmotionType::kHappy:
            return "happy";
            break;
        case EmotionType::kCalm:
            return "calm";
            break;
        case EmotionType::kSatisfied:
            return "satisfied";
            break;
        case EmotionType::kBrave:
            return "brave";
            break;
        case EmotionType::kExtroverted:
            return "extroverted";
            break;
        }
        return "none";
    }

    std::string Emotion::ToString()
    {
        return fmt::format("{} was {} with a value of {}", owner_.lock()->name_, name_, value_);
    }
} // namespace tale