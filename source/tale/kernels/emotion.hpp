#ifndef TALE_EMOTION_H
#define TALE_EMOTION_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    enum class EmotionType
    {
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
    class Emotion : Kernel
    {
    public:
        Emotion(std::vector<std::weak_ptr<Kernel>> reasons);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_EMOTION_H