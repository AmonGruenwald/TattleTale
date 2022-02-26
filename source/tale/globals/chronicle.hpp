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
        Chronicle(Random &random);
        std::weak_ptr<Interaction> CreateInteraction(const InteractionPrototype &prototype, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants);
        std::weak_ptr<Emotion> CreateEmotion(EmotionType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Relationship> CreateRelationship(RelationshipType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Resource> CreateResource(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        std::weak_ptr<Goal> CreateGoal(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        std::weak_ptr<Trait> CreateTrait(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);

        size_t GetKernelAmount();
        std::string GetRandomCausalityChainDescription(size_t depth);

    private:
        Random &random_;
        std::vector<std::shared_ptr<Kernel>>
            kernels_;

        std::string GetRecursiveKernelDescription(std::weak_ptr<Kernel> kernel, size_t current_depth, size_t max_depth);
    };
} // namespace tale
#endif // TALE_GLOBALS_CHRONICLE_H