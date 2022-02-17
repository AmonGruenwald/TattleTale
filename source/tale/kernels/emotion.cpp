#include "tale/kernels/emotion.hpp"
#include <assert.h>
#include <iostream>
#include <fmt/core.h>

namespace tale
{
    Emotion::Emotion(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value) : Kernel(name, tick, reasons), value_(value){};

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

    float Emotion::GetValue() const
    {
        return value_;
    }

    std::string Emotion::ToString()
    {
        return fmt::format("{}: {}", name_, value_);
    }
} // namespace tale