#ifndef TALE_KERNELS_RESOURCEKERNELS_EMOTION_H
#define TALE_KERNELS_RESOURCEKERNELS_EMOTION_H

#include <string>
#include "tale/kernels/resourcekernels/resource.hpp"

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
     * @brief Represents an Emotion an Actor has.
     *
     * Will be instantiated inside a map connecting different \link EmotionType EmotionTypes \endlink to different \link Emotion Emotions \endlink for each Actor.
     *
     */
    class Emotion : public Resource
    {
    public:
        /**
         * @brief Converts a string to an EmotionType.
         *
         * Uses the same string values EmotionTypeToString returns.
         * So valid strings are: "happy" "calm", "satisfied", "brave" and "extroverted".
         * Everything else just return EmotionType::kNone. (But an assert will crash the Application in Debug mode.)
         *
         * @param emotion_string The string we want to convert.
         * @return The corresponding EmotionType value.
         */
        static EmotionType StringToEmotionType(std::string emotion_string);
        /**
         * @brief Converts an EmotionType to a string for easy printing.
         *
         * Note that EmotionType:kNone and EmotionType::kLast will assert and crash in Debug mode.
         *
         * @param emotion_type The EmotionType we want to convert.
         * @return The corresponding string.
         */
        static std::string EmotionTypeToString(EmotionType emotion_type);

        /**
         * @brief Creates a string describing the Emotion for easy printing.
         *
         * @return The description string.
         */
        std::string ToString() override;

    private:
        /**
         * @brief Constructor initializing base Resource class and type_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param type What EmotionType this Emotion is of.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Emotion was created.
         * @param owner The Actor which owns this Resource.
         * @param reasons What other \link Kernel Kernels \endlink  led to this Emotion and its value.
         * @param value Value of the Emotion between -1.0 and 1.0.
         */
        Emotion(EmotionType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        /**
         * @brief The EmotionType of this Emotion.
         */
        EmotionType type_;
        friend class Chronicle;
    };

} // namespace tale
#endif // TALE_KERNELS_RESOURCEKERNELS_EMOTION_H