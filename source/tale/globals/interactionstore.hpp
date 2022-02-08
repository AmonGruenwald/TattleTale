#ifndef TALE_GLOBALS_INTERACTIONSTORE_H
#define TALE_GLOBALS_INTERACTIONSTORE_H

#include <string>
#include <map>
#include "tale/kernels/interaction.hpp"

namespace tale
{
    /**
     * @brief Stores Prototypes for all possible interactions.
     */
    class InteractionStore
    {
    public:
        InteractionStore();
        std::string GetRandomInteractionName();
        Interaction CloneInteraction();

    private:
        std::map<std::string, Interaction> prototype_catalogue_;
        std::string prototype_json_path_ = "tale/resources/interactionprototypes.json";
    };

} // namespace tale
#endif // TALE_GLOBALS_INTERACTIONSTORE_H