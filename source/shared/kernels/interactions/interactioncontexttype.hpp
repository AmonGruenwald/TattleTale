#ifndef TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H
#define TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H
#include <string>
namespace tale
{
    /**
     * @brief Types of Context an Interaction can happen in.
     */
    enum class ContextType
    {
        kNone,
        kCourse,
        kFreetime,
        kLast
    };

    /**
     * @brief Converts a string to an ContextType.
     *
     * Uses the same string values ContextTypeToString returns.
     * So valid strings are: "course" and"freetime".
     * Everything else just return ContextType::kNone.
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
        return ContextType::kNone;
    }
    /**
     * @brief Converts an ContextType to a string for easy printing.
     *
     * @param context_type The ContextType we want to convert.
     * @return The corresponding string.
     */
    inline std::string ContextTypeToString(ContextType context_type)
    {
        switch (context_type)
        {
        case ContextType::kNone:
            return "none";
            break;
        case ContextType::kLast:
            return "last";
            break;
        case ContextType::kCourse:
            return "course";
            break;
        case ContextType::kFreetime:
            return "freetime";
            break;
        }
        return "none";
    }
} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONCONTEXTTYPE_H