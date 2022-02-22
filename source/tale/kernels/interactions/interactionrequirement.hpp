#ifndef TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H
#define TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H

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
    };

} // namespace tale
#endif // TALE_INTERACTIONS_INTERACTIONREQUIREMENT_H