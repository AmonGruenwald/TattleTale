#ifndef TALE_EMOTION_H
#define TALE_EMOTION_H

#include <string>
#include "tale/kernels/kernel.hpp"

namespace tale
{
    enum class EmotionType
    {
        kNone,
        kHappy,
        kCalm,
        kSatisfied,
        kBrave,
        kExtroverted,
        kLast
    };

    /**
     * @brief Represents the Emotions an Actor has.
     *
     */
    class Emotion : public Kernel
    {
    public:
        Emotion(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        static EmotionType StringToEmotionType(std::string emotion_string);
        static std::string EmotionTypeToString(EmotionType emotion_type);
        float GetValue() const;
        std::string ToString();

    private:
        float value_;
    };

} // namespace tale
#endif // TALE_EMOTION_H