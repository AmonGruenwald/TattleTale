#include "shared/chronicle.hpp"
#include "shared/tattletalecore.hpp"
#include "shared/actor.hpp"

namespace tattletale
{
    Chronicle::Chronicle(Random &random) : random_(random){};
    void Chronicle::Reset(size_t actor_count)
    {
        kernels_by_actor_.clear();
        all_kernels_.clear();
        all_interactions_.clear();
        for (size_t i = 0; i < actor_count; ++i)
        {
            kernels_by_actor_.push_back(std::vector<std::shared_ptr<Kernel>>());
        }
    }
    std::weak_ptr<Interaction> Chronicle::CreateInteraction(
        const std::shared_ptr<InteractionPrototype> prototype,
        const std::shared_ptr<InteractionRequirement> requirement,
        const std::shared_ptr<InteractionTendency> tendency,
        float chance,
        size_t tick,
        std::vector<std::weak_ptr<Kernel>> reasons,
        std::vector<std::weak_ptr<Actor>> participants)
    {
        std::shared_ptr<Interaction> interaction(new Interaction(prototype, requirement, tendency, chance, all_kernels_.size(), tick, reasons, participants));
        all_kernels_.push_back(interaction);
        for (auto &owner : participants)
        {
            kernels_by_actor_[owner.lock()->id_].push_back(interaction);
        }
        all_interactions_.push_back(interaction);
        return interaction;
    }
    std::weak_ptr<Emotion> Chronicle::CreateEmotion(EmotionType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Emotion> emotion(new Emotion(type, all_kernels_.size(), tick, owner, reasons, value));
        all_kernels_.push_back(emotion);
        kernels_by_actor_[owner.lock()->id_].push_back(emotion);
        return emotion;
    }
    std::weak_ptr<Relationship> Chronicle::CreateRelationship(RelationshipType type, size_t tick, std::weak_ptr<Actor> owner, std::weak_ptr<Actor> target, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Relationship> relationship(new Relationship(type, all_kernels_.size(), tick, owner, target, reasons, value));
        all_kernels_.push_back(relationship);
        kernels_by_actor_[owner.lock()->id_].push_back(relationship);
        return relationship;
    }
    std::weak_ptr<Resource> Chronicle::CreateResource(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value)
    {
        std::shared_ptr<Resource> resource(new Resource(name, all_kernels_.size(), tick, owner, reasons, value));
        all_kernels_.push_back(resource);
        kernels_by_actor_[owner.lock()->id_].push_back(resource);
        return resource;
    }
    std::weak_ptr<Goal> Chronicle::CreateGoal(GoalType type, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        std::shared_ptr<Goal> goal(new Goal(type, all_kernels_.size(), tick, owner, reasons));
        all_kernels_.push_back(goal);
        kernels_by_actor_[owner.lock()->id_].push_back(goal);
        return goal;
    }
    std::weak_ptr<Trait> Chronicle::CreateTrait(std::string name, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons)
    {
        std::shared_ptr<Trait> trait(new Trait(name, all_kernels_.size(), tick, owner, reasons));
        all_kernels_.push_back(trait);
        kernels_by_actor_[owner.lock()->id_].push_back(trait);
        return trait;
    }

    float Chronicle::GetAverageInteractionChance() const
    {
        float sum = 0;
        for (auto interaction : all_interactions_)
        {
            sum += interaction->chance_;
        }
        return (sum / all_interactions_.size());
    }

    std::string Chronicle::GetKnownActorsDescription(size_t actor_id) const
    {
        std::string description;
        std::shared_ptr<Actor> actor = actors_[actor_id];
        std::vector<std::weak_ptr<tattletale::Actor>> known_actors = actor->GetAllKnownActors();
        for (auto &other_actor : known_actors)
        {
            description += (other_actor.lock()->name_ + " known with value " + std::to_string(actor->CalculateRelationshipValue(other_actor.lock()->id_)) + "\n");
        }
        return description;
    }
    size_t Chronicle::GetKernelAmount() const
    {
        return all_kernels_.size();
    }

    std::string Chronicle::GetRandomCausalityChainDescription(size_t depth) const
    {
        if (all_kernels_.size() <= 0)
        {
            return "";
        }
        auto kernel = all_kernels_[random_.GetUInt(0, all_kernels_.size() - 1)];
        return GetRecursiveKernelDescription(kernel, 0, depth);
    }

    std::string Chronicle::GetKissingCausalityChainDescription(size_t depth) const
    {
        if (all_kernels_.size() <= 0)
        {
            return "";
        }
        std::vector<std::shared_ptr<Kernel>> possible_kernels;
        for (auto kernel : all_kernels_)
        {
            if (kernel->name_ == "Kiss successfully")
            {
                possible_kernels.push_back(kernel);
            }
        }
        if (possible_kernels.size() > 0)
        {
            auto kernel = possible_kernels[random_.GetUInt(0, possible_kernels.size() - 1)];
            return GetRecursiveKernelDescription(kernel, 0, depth);
        }
        return "Did not find a kiss.";
    }

    std::string Chronicle::GetActorInteractionsDescription(size_t id) const
    {
        if (actors_.size() < id)
        {
            return "No Actor of this ID exists.";
        }
        auto &kernels = kernels_by_actor_[id];
        if (kernels.size() <= 0)
        {
            return "No Kernels were created for this ID.";
        }
        std::string description = "Interactions for " + actors_[id]->name_;
        for (size_t i = 0; i < kernels.size(); ++i)
        {
            if (std::dynamic_pointer_cast<Interaction>(kernels[i]))
            {
                description += "\n";
                description += kernels[i]->ToString();
            }
        }
        return description;
    }

    std::weak_ptr<Interaction> Chronicle::FindUnlikeliestInteraction() const
    {
        std::shared_ptr<Interaction> unlikeliest_interaction = all_interactions_[0];
        for (size_t i = 1; i < all_interactions_.size(); ++i)
        {
            std::shared_ptr<Interaction> current_interaction = all_interactions_[i];
            if (current_interaction->chance_ < unlikeliest_interaction->chance_)
            {
                unlikeliest_interaction = current_interaction;
            }
        }
        return unlikeliest_interaction;
    }
    std::string Chronicle::GetGoalCausalityChainDescription(size_t depth) const
    {
        if (all_kernels_.size() <= 0)
        {
            return "";
        }
        std::vector<std::shared_ptr<Kernel>> possible_kernels;
        for (auto kernel : all_kernels_)
        {
            for (auto reason : kernel->reasons_)
            {
                if (std::dynamic_pointer_cast<Goal>(reason.lock()))
                {
                    possible_kernels.push_back(kernel);
                    break;
                }
            }
        }
        if (possible_kernels.size() > 0)
        {
            auto kernel = possible_kernels[random_.GetUInt(0, possible_kernels.size() - 1)];
            return GetRecursiveKernelDescription(kernel, 0, depth);
        }
        return "Did not find a kernel with a goal as reason.";
    }

    std::string Chronicle::GetRecursiveKernelDescription(std::weak_ptr<Kernel> kernel, size_t current_depth, size_t max_depth) const
    {
        std::shared_ptr<Kernel> locked_kernel = kernel.lock();
        std::string description = "D" + std::to_string(current_depth);
        for (size_t i = 0; i < current_depth; ++i)
        {
            description += "-";
        }
        description += ":" + locked_kernel->ToString() + " (T" + std::to_string(kernel.lock()->tick_) + ")\n";
        if (current_depth < max_depth)
        {
            for (size_t i = 0; i < current_depth; ++i)
            {
                description += " ";
            }

            if (kernel.lock()->reasons_.size() > 0)
            {
                description += "   Because: \n";
                for (auto &reason : kernel.lock()->reasons_)
                {
                    description += GetRecursiveKernelDescription(reason, current_depth + 1, max_depth);
                }
            }
            else
            {
                description += "   For no reason.\n";
            }
        }
        return description;
    }
} // namespace tattletale