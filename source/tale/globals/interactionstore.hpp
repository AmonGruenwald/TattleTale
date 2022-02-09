#ifndef TALE_GLOBALS_INTERACTIONSTORE_H
#define TALE_GLOBALS_INTERACTIONSTORE_H

#include <string>
#include <map>
#include <memory>
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
        const size_t &GetParticipantCount(std::string interaction_name);
        const std::vector<float> &GetResourceEffects(std::string interaction_name);
        const std::vector<std::map<EmotionType, float>> &GetEmotionEffects(std::string interaction_name);
        const std::vector<std::map<size_t, std::map<RelationshipType, float>>> &GetRelationshipEffects(std::string interaction_name);
        std::shared_ptr<Interaction> CreateInteraction(std::string interaction_name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants);

    private:
        std::map<std::string, Interaction> prototype_catalogue_;
        std::string prototype_json_path_ = "tale/resources/interactionprototypes.json";
    };

} // namespace tale
#endif // TALE_GLOBALS_INTERACTIONSTORE_H