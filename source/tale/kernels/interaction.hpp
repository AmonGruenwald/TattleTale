#ifndef TALE_INTERACTION_H
#define TALE_INTERACTION_H

#include "tale/kernels/kernel.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/actor.hpp"

namespace tale
{
    /**
     * @brief Represents an Interaction an Actor had with another Actor
     *
     */
    class Interaction : public Kernel
    {
    public:
        size_t participant_count_;
        std::vector<std::weak_ptr<Actor>> participants_;
        std::vector<float> resource_effects_;
        std::vector<std::map<EmotionType, float>> emotion_effects_;
        // For each actor hold a map which maps to participants a map of relationship type value pais
        std::vector<std::map<size_t, std::map<RelationshipType, float>>> relationship_effects_;

        Interaction(
            std::string name,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            size_t participant_count,
            std::vector<std::weak_ptr<Actor>> participants,
            std::vector<float> resource_effects,
            std::vector<std::map<EmotionType, float>> emotion_effects,
            std::vector<std::map<size_t, std::map<RelationshipType, float>>> relationship_effects);

        std::string ToString();

    private:
        Interaction();
        friend class InteractionStore;
    };

} // namespace tale
#endif // TALE_INTERACTION_H