#ifndef TALE_INTERACTIONS_REQUIREMENT_H
#define TALE_INTERACTIONS_REQUIREMENT_H

#include <math.h>
#include <assert.h>
#include <string>
#include <algorithm>
namespace tale
{
    enum class ContextType
    {
        kNone,
        kCourse,
        kFreetime,
        kLast
    };
    /**
     * @brief Stores all necessary \link Requirement Requirements \endlink for an Interaction to happen.
     */
    struct Requirement
    {
        std::string name = "";
        ContextType context = ContextType::kNone;
        size_t participant_count = 1;
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
        static ContextType StringToContextType(std::string context_string)
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
        static std::string ContextTypeToString(ContextType context_type)
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
    };

} // namespace tale
#endif // TALE_INTERACTIONS_REQUIREMENT_H