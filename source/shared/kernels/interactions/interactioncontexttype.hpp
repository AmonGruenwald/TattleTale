#ifndef TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H
#define TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H
#include <string>
namespace tattletale
{
    /**
     * @brief Types of Context an Interaction can happen in.
     */
    enum class ContextType
    {
        kCourse,
        kFreetime,
        kLast
    };

    /**
     * @brief Converts a string to an ContextType.
     *
     * Uses the same string values formater returns.
     * So valid strings are: "course" and"freetime".
     * Everything else just returns ContextType::kLast.
     *
     * @param context_string The string we want to convert.
     * @return The corresponding ContextType value.
     */
    inline ContextType StringToContextType(std::string context_string)
    {
        if (context_string == "course")
            return ContextType::kCourse;
        if (context_string == "freetime")
            return ContextType::kFreetime;
        return ContextType::kLast;
    }

} // namespace tattletale

template <>
struct fmt::formatter<tattletale::ContextType> : formatter<string_view>
{
    std::string context_type_names[3] = {
        "course",
        "freetime",
        "last"};
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(tattletale::ContextType type, FormatContext &ctx)
    {
        size_t enum_index = static_cast<size_t>(type);
        string_view name = context_type_names[enum_index];
        return formatter<string_view>::format(name, ctx);
    }
};
#endif // TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H