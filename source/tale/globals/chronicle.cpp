#include "tale/globals/chronicle.hpp"
#include "tale/talecore.hpp"

namespace tale
{
    Chronicle::Chronicle(Random &random) : random_(random) {}
    std::weak_ptr<Interaction> Chronicle::CreateInteraction(const InteractionPrototype &prototype, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants)
    {
        std::shared_ptr<Interaction> interaction(new Interaction(prototype, kernels_.size(), tick, reasons, participants));
        kernels_.push_back(interaction);
        return interaction;
    }
    std::weak_ptr<Emotion> Chronicle::CreateEmotion(EmotionType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Emotion> emotion(new Emotion(type, kernels_.size(), tick, owner, reasons, value));
        kernels_.push_back(emotion);
        return emotion;
    }
    std::weak_ptr<Relationship> Chronicle::CreateRelationship(RelationshipType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Relationship> relationship(new Relationship(type, kernels_.size(), tick, owner, reasons, value));
        kernels_.push_back(relationship);
        return relationship;
    }
    std::weak_ptr<Resource> Chronicle::CreateResource(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Resource> resource(new Resource(name, kernels_.size(), tick, owner, reasons, value));
        kernels_.push_back(resource);
        return resource;
    }
    std::weak_ptr<Goal> Chronicle::CreateGoal(GoalType type, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        std::shared_ptr<Goal> trait(new Goal(type, kernels_.size(), tick, reasons));
        kernels_.push_back(trait);
        return trait;
    }
    std::weak_ptr<Trait> Chronicle::CreateTrait(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        std::shared_ptr<Trait> trait(new Trait(name, kernels_.size(), tick, reasons));
        kernels_.push_back(trait);
        return trait;
    }

    size_t Chronicle::GetKernelAmount()
    {
        return kernels_.size();
    }

    std::string Chronicle::GetRandomCausalityChainDescription(size_t depth)
    {
        if (kernels_.size() <= 0)
        {
            return "";
        }
        auto kernel = kernels_[random_.GetUInt(0, kernels_.size() - 1)];
        return GetRecursiveKernelDescription(kernel, 0, depth);
    }

    std::string Chronicle::GetRecursiveKernelDescription(std::weak_ptr<Kernel> kernel, size_t current_depth, size_t max_depth)
    {
        std::shared_ptr<Kernel> locked_kernel = kernel.lock();
        std::string description = "D" + std::to_string(current_depth);
        for (size_t i = 0; i < current_depth; ++i)
        {
            description += "-";
        }
        description += ":" + locked_kernel->ToString() + "\n";
        if (current_depth < max_depth && kernel.lock()->reasons_.size() > 0)
        {
            for (size_t i = 0; i < current_depth; ++i)
            {
                description += " ";
            }
            description += "   Because: \n";
            for (auto &reason : kernel.lock()->reasons_)
            {
                description += GetRecursiveKernelDescription(reason, current_depth + 1, max_depth);
            }
        }
        return description;
    }
} // namespace tale