#include "shared/kernels/resourcekernels/emotion.hpp"
#include <assert.h>
#include <iostream>
#include <fmt/core.h>
#include "shared/actor.hpp"

namespace tattletale
{
    Emotion::Emotion(EmotionType type, size_t id, size_t tick, Actor *owner, std::vector<Kernel *> reasons, float value)
        : Resource(
              fmt::format("{}", type),
              Emotion::positive_name_variants_[static_cast<int>(type)],
              Emotion::negative_name_variants_[static_cast<int>(type)],
              id,
              tick,
              owner,
              reasons,
              value,
              KernelType::kEmotion,
              Verb("felt", "feeling", "feel")),
          type_(type){};

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
        TATTLETALE_ERROR_PRINT(true, "Invalid Emotion string was passed");
        return EmotionType::kLast;
    }
    EmotionType Emotion::GetType()
    {
        return type_;
    }
} // namespace tattletale