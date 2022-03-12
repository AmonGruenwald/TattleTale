#ifndef TALE_GLOBALS_CHRONICLE_H
#define TALE_GLOBALS_CHRONICLE_H

#include <memory>
#include <vector>
#include <string>
#include "tale/kernels/interactions/interaction.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include "tale/kernels/goal.hpp"
#include "tale/kernels/trait.hpp"
#include "tale/globals/random.hpp"

namespace tale
{
    class Chronicle
    {
    public:
        /**
         * @brief Holds all instanced \link Actor Actors \endlink.
         */
        std::vector<std::shared_ptr<Actor>> actors_;

        Chronicle(Random &random, size_t actor_count);
        std::weak_ptr<Interaction> CreateInteraction(
            const InteractionPrototype &prototype,
            const Requirement &requirement,
            const Tendency &tendency,
            float chance,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            std::vector<std::weak_ptr<Actor>> participants);
        std::weak_ptr<Emotion> CreateEmotion(EmotionType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Relationship> CreateRelationship(RelationshipType type, size_t tick, std::weak_ptr<Actor> owner, std::weak_ptr<Actor> target, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Resource> CreateResource(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Goal> CreateGoal(GoalType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);
        std::weak_ptr<Trait> CreateTrait(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);

        float GetAverageInteractionChance() const;
        size_t GetKernelAmount() const;
        std::string GetRandomCausalityChainDescription(size_t depth) const;
        std::string GetKissingCausalityChainDescription(size_t depth) const;
        std::string GetGoalCausalityChainDescription(size_t depth) const;
        std::string GetActorInteractionsDescription(size_t id) const;

    private:
        Random &random_;
        std::vector<std::shared_ptr<Kernel>>
            all_kernels_;
        std::vector<std::shared_ptr<Interaction>>
            all_interactions_;
        std::vector<std::vector<std::shared_ptr<Kernel>>>
            kernels_by_actor_;

        std::string GetRecursiveKernelDescription(std::weak_ptr<Kernel> kernel, size_t current_depth, size_t max_depth) const;
    };
} // namespace tale
#endif // TALE_GLOBALS_CHRONICLE_H