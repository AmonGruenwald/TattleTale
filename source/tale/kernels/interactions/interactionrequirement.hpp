#ifndef TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H
#define TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H

#include <fmt/core.h>
#include "tale/kernels/interactions/interactioncontexttype.hpp"
namespace tale
{
    /**
     * @brief Stores all necessary \link Requirement Requirements \endlink for an Interaction to happen.
     */
    struct Requirement
    {
        /**
         * @brief ContextType the corresponding Interaction needs.
         */
        ContextType context = ContextType::kNone;
        /**
         * @brief How many \link Actor Actors \endlink the corresponding Interaction needs.
         */
        size_t participant_count = 1;

        std::string ToString()
        {
            std::string context_string = fmt::format("Context: {}\n", ContextTypeToString(context));
            std::string participant_count_string = fmt::format("Participant Count: {}\n", participant_count);
            return (fmt::format("{}\n{}", context_string, participant_count_string));
        }

        void ClearValues()
        {
            context = ContextType::kNone;
            participant_count = 1;
        }
    };

} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H