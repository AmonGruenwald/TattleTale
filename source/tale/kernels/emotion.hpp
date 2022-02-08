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
        kExtroverted
    };

    /**
     * @brief Represents the Emotions an Actor has.
     *
     */
    class Emotion : public Kernel
    {
    public:
        Emotion(std::string name, std::vector<std::weak_ptr<Kernel>> reasons);
        static EmotionType StringToEmotionType(std::string emotion_string);
        static std::string EmotionTypeToString(EmotionType emotion_type);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_EMOTION_H