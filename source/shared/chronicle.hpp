#ifndef TALE_GLOBALS_CHRONICLE_H
#define TALE_GLOBALS_CHRONICLE_H

#include <memory>
#include <vector>
#include <string>
#include "shared/kernels/interactions/interaction.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/kernels/goal.hpp"
#include "shared/kernels/trait.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    class Chronicle
    {
    public:
        /**
         * @brief Holds all instanced \link Actor Actors \endlink.
         */
        std::vector<std::shared_ptr<Actor>> actors_;

        Chronicle(Random &random);
        void Reset(size_t actor_count);
        std::weak_ptr<Interaction> CreateInteraction(
            const std::shared_ptr<InteractionPrototype> prototype,
            const std::shared_ptr<InteractionRequirement> requirement,
            const std::shared_ptr<InteractionTendency> tendency,
            float chance,
            size_t tick,
            std::vector<std::weak_ptr<Kernel>> reasons,
            std::vector<std::weak_ptr<Actor>> participants);
        std::weak_ptr<Emotion> CreateEmotion(EmotionType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Relationship> CreateRelationship(RelationshipType type, size_t tick, std::weak_ptr<Actor> owner, std::weak_ptr<Actor> target, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Resource> CreateResource(std::string name, std::string positive_name_variant, std::string negative_name_variant, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Goal> CreateGoal(GoalType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);
        std::weak_ptr<Trait> CreateTrait(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);

        float GetAverageInteractionChance() const;
        float GetAverageInteractionReasonCount() const;
        std::string GetKnownActorsDescription(size_t actor_id) const;
        size_t GetKernelAmount() const;
        std::string GetRandomCausalityChainDescription(size_t depth) const;
        std::string GetKissingCausalityChainDescription(size_t depth) const;
        std::string GetGoalCausalityChainDescription(size_t depth) const;
        std::string GetActorInteractionsDescription(size_t id) const;
        std::weak_ptr<Interaction> FindUnlikeliestInteraction(size_t tick_cutoff) const;
        std::shared_ptr<Interaction> FindMostOccuringInteractionPrototypeForActor(size_t actor_id) const;
        std::weak_ptr<Emotion> GetLastEmotionOfType(size_t tick, size_t actor_id, EmotionType type) const;
        std::weak_ptr<Resource> GetLastWealth(size_t tick, size_t actor_id) const;
        size_t GetLastTick() const;

    private:
        Random &random_;
        std::vector<std::shared_ptr<Kernel>>
            all_kernels_;
        std::vector<std::shared_ptr<Interaction>>
            all_interactions_;
        std::vector<std::vector<std::shared_ptr<Interaction>>>
            interactions_by_actor_;
        std::vector<std::vector<std::shared_ptr<Kernel>>>
            kernels_by_actor_;
        std::vector<std::vector<std::shared_ptr<Emotion>>>
            emotions_by_actor_;
        std::vector<std::vector<std::shared_ptr<Resource>>>
            wealth_by_actor_;
        size_t highest_interaction_id = 0;
        std::string GetRecursiveKernelDescription(std::weak_ptr<Kernel> kernel, size_t current_depth, size_t max_depth) const;
    };
} // namespace tattletale
#endif // TALE_GLOBALS_CHRONICLE_H