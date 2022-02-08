#include "tale/kernels/emotion.hpp"
#include <assert.h>
#include <iostream>

namespace tale
{
    Emotion::Emotion(std::string name, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, reasons){};
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
        assert(emotion_type != EmotionType::kNone); // invalid enum was passed
        switch (emotion_type)
        {
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
        return "I am an Emotion.\n";
    }
} // namespace tale