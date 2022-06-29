#ifndef TALE_GLOBALS_CHRONICLE_H
#define TALE_GLOBALS_CHRONICLE_H

#include <memory>
#include <list>
#include <vector>
#include <string>
#include "shared/kernels/interactions/interaction.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/kernels/goal.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    struct ActorStatus{
        Goal* goal;
        Resource* wealth;
        std::vector<Emotion*> emotions;
    };
    class School;
    class Chronicle
    {
    public:
        /**
         * @brief Holds all instanced \link Actor Actors \endlink.
         */
        std::vector<Actor *> actors_;

        Chronicle(Random &random);
        ~Chronicle();
        void Reset();
        Actor *CreateActor(School &school, std::string first_name, std::string last_name);

        Interaction *CreateInteraction(
            const std::shared_ptr<InteractionPrototype> prototype,
            const std::shared_ptr<InteractionRequirement> requirement,
            const std::shared_ptr<InteractionTendency> tendency,
            float chance,
            size_t tick,
            std::vector<Kernel *> reasons,
            std::vector<Actor *> participants);
        Emotion *CreateEmotion(EmotionType type, size_t tick, Actor *owner, std::vector<Kernel *> reasons, float value);
        Relationship *CreateRelationship(RelationshipType type, size_t tick, Actor *owner, Actor *target, std::vector<Kernel *> reasons, float value);
        Resource *CreateResource(std::string name, std::string positive_name_variant, std::string negative_name_variant, size_t tick, Actor *owner, std::vector<Kernel *> reasons, float value);
        Goal *CreateGoal(GoalType type, size_t tick, Actor *owner, std::vector<Kernel *> reasons);

        std::vector<std::vector<Kernel *>> GetEveryPossibleChain(size_t chain_size) const;
        float GetAverageInteractionChance() const;
        float GetAverageInteractionReasonCount() const;
        std::string GetKnownActorsDescription(size_t actor_id) const;
        size_t GetKernelAmount() const;
        std::string GetRandomCausalityChainDescription(size_t depth) const;
        std::string GetKissingCausalityChainDescription(size_t depth) const;
        std::string GetGoalCausalityChainDescription(size_t depth) const;
        std::string GetActorInteractionsDescription(size_t id) const;
        Interaction *FindUnlikeliestInteraction(size_t tick_cutoff) const;
        Interaction *FindMostOccuringInteractionPrototypeForActor(size_t actor_id) const;
        ActorStatus FindActorStatusDuringTick(size_t actor_id, size_t tick) const;
        size_t RecursivelyFindHighestAbsoluteInterestChain(Kernel *kernel, size_t current_depth, size_t max_depth, std::vector<Kernel *> &out_chain) const;
        Emotion *GetLastEmotionOfType(size_t tick, size_t actor_id, EmotionType type) const;
        Resource *GetLastWealth(size_t tick, size_t actor_id) const;
        size_t GetLastTick() const;
        Random &GetRandom() const;

    private:
        Random &random_;
        std::vector<Kernel *>
            all_kernels_;
        std::vector<Interaction *>
            all_interactions_;
        std::vector<std::vector<Interaction *>>
            interactions_by_actor_;
        std::vector<std::vector<Kernel *>>
            kernels_by_actor_;
        std::vector<std::vector<Emotion *>>
            emotions_by_actor_;
        std::vector<std::vector<Resource *>>
            wealth_by_actor_;
        size_t highest_interaction_id = 0;
        std::vector<std::vector<Kernel *>> GetEveryPossibleChainRecursivly(Kernel *kernel, size_t current_depth, size_t max_depth) const;
        std::string GetRecursiveKernelDescription(Kernel *kernel, size_t current_depth, size_t max_depth) const;
    };
} // namespace tattletale
#endif // TALE_GLOBALS_CHRONICLE_H