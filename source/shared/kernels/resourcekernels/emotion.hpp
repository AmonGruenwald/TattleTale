#ifndef TALE_KERNELS_RESOURCEKERNELS_EMOTION_H
#define TALE_KERNELS_RESOURCEKERNELS_EMOTION_H

#include <string>
#include "shared/kernels/resourcekernels/resource.hpp"

namespace tattletale
{
    /**
     * @brief Types of Emotion axis for an Actor.
     */
    enum class EmotionType
    {
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
     * Will be instantiated inside a vector connecting different \link EmotionType EmotionTypes \endlink (converted to indices)
     * to different \link Emotion Emotions \endlink for each Actor.
     *
     */
    class Emotion : public Resource
    {
    public:
        /**
         * @brief Converts a string to an EmotionType.
         *
         * Uses the same string values formater returns.
         * So valid strings are: "happy" "calm", "satisfied", "brave" and "extroverted".
         * Everything else just return EmotionType::kLast. (But an assert will crash the Application in Debug mode.)
         *
         * @param emotion_string The string we want to convert.
         * @return The corresponding EmotionType value.
         */
        static EmotionType StringToEmotionType(std::string emotion_string);

        /**
         * @brief Getter for the EmotionType of this Emotion.
         *
         * @return The EmotionType.
         */
        EmotionType GetType();

        // TODO: documentation
        virtual float CalculateChanceInfluence(const Interaction *interaction) const override;
        // TODO: documentation
        virtual bool IsSameSpecificType(Kernel *other) const;

    private:
        /**
         * @brief Constructor, also initializing base Resource class and type_ member.
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
        Emotion(EmotionType type, size_t id, size_t tick, Actor *owner, std::vector<Kernel *> reasons, float value);
        /**
         * @brief The EmotionType of this Emotion.
         */
        EmotionType type_;
        /**
         * @brief Vector containing the positive name variants for this emotion.
         */
        const static inline std::vector<std::string> positive_name_variants_ =
            {"happy",
             "calm",
             "satisfied",
             "brave",
             "extroverted"};
        /**
         * @brief Vector containing the negative name variants for this emotion.
         */
        const static inline std::vector<std::string> negative_name_variants_ =
            {"sad",
             "stressed",
             "unfulfilled",
             "fearful",
             "shy"};
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
        friend class Chronicle;
    };

} // namespace tattletale

template <>
struct fmt::formatter<tattletale::EmotionType> : formatter<string_view>
{
    std::string emotion_type_names[6] = {
        "happy",
        "calm",
        "satisfied",
        "brave",
        "extroverted",
        "last"};
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(tattletale::EmotionType type, FormatContext &ctx)
    {
        size_t enum_index = static_cast<size_t>(type);
        string_view name = emotion_type_names[enum_index];
        return formatter<string_view>::format(name, ctx);
    }
};
#endif // TALE_KERNELS_RESOURCEKERNELS_EMOTION_H
