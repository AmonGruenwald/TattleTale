#include "tale/globals/chronicle.hpp"
#include "tale/talecore.hpp"

namespace tale
{
    std::weak_ptr<Interaction> Chronicle::CreateInteraction(const InteractionPrototype &prototype, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, std::vector<std::weak_ptr<Actor>> participants)
    {
        std::shared_ptr<Interaction> interaction(new Interaction(prototype, kernels_.size(), tick, reasons, participants));
        kernels_.push_back(interaction);
        return interaction;
    }
    std::weak_ptr<Emotion> Chronicle::CreateEmotion(EmotionType type, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Emotion> emotion(new Emotion(type, kernels_.size(), tick, reasons, value));
        kernels_.push_back(emotion);
        return emotion;
    }
    std::weak_ptr<Relationship> Chronicle::CreateRelationship(RelationshipType type, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Relationship> relationship(new Relationship(type, kernels_.size(), tick, reasons, value));
        kernels_.push_back(relationship);
        return relationship;
    }
    std::weak_ptr<Resource> Chronicle::CreateResource(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Resource> resource(new Resource(name, kernels_.size(), tick, reasons, value));
        kernels_.push_back(resource);
        return resource;
    }
    std::weak_ptr<Goal> Chronicle::CreateGoal(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        std::shared_ptr<Goal> trait(new Goal(name, kernels_.size(), tick, reasons));
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
} // namespace tale