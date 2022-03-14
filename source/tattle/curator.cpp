#include "tattle/curator.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include "shared/tattletalecore.hpp"

namespace tattletale
{
    Curator::Curator(const Chronicle &chronicle, const Setting &setting) : chronicle_(chronicle), setting_(setting) {}

    std::shared_ptr<Kernel> Curator::RecursivelyFindUnlikeliestReason(std::shared_ptr<Kernel> to_check, std::shared_ptr<Kernel> current_best)
    {
        if (to_check->GetChance() < current_best->GetChance())
        {
            current_best = to_check;
        }
        auto &reasons = to_check->GetReasons();
        for (auto &reason : reasons)
        {
            auto locked_reason = reason.lock();
            current_best = RecursivelyFindUnlikeliestReason(locked_reason, current_best);
        }
        return current_best;
    }
    std::shared_ptr<Kernel> Curator::RecursivelyFindUnlikeliestConsequence(std::shared_ptr<Kernel> to_check, std::shared_ptr<Kernel> current_best, size_t depth)
    {
        if (!current_best || to_check->GetChance() < current_best->GetChance())
        {
            current_best = to_check;
        }
        if (depth > 0)
        {
            auto &consequences = to_check->GetConsequences();
            for (auto &consequence : consequences)
            {
                current_best = RecursivelyFindUnlikeliestConsequence(consequence.lock(), current_best, depth - 1);
            }
        }
        return current_best;
    }
    std::vector<std::shared_ptr<Kernel>> Curator::FindCausalConnection(std::shared_ptr<Kernel> start, std::shared_ptr<Kernel> end) const
    {
        std::vector<std::shared_ptr<Kernel>> causal_chain;
        RecursivelyFindCausalConnectionBackwards(end, start, causal_chain);
        return causal_chain;
    }

    bool Curator::RecursivelyFindCausalConnectionBackwards(std::shared_ptr<Kernel> root, std::shared_ptr<Kernel> start, std::vector<std::shared_ptr<Kernel>> &out_causal_chain) const
    {
        if (root->id_ == start->id_)
        {
            out_causal_chain.push_back(root);
            return true;
        }
        if (root->tick_ < start->tick_)
        {
            return false;
        }
        for (auto &reason : root->GetReasons())
        {
            if (RecursivelyFindCausalConnectionBackwards(reason.lock(), start, out_causal_chain))
            {
                out_causal_chain.push_back(root);
                return true;
            }
        }
        return false;
    }
    std::string Curator::Curate()
    {
        size_t depth = 20;
        size_t base_interaction_tick_distance_to_end = 10;
        size_t tick_cutoff = chronicle_.GetLastTick();
        tick_cutoff = tick_cutoff > depth ? tick_cutoff - base_interaction_tick_distance_to_end : tick_cutoff;
        auto base_interaction = chronicle_.FindUnlikeliestInteraction(tick_cutoff).lock();
        std::shared_ptr<Kernel> goal_kernel(nullptr);
        auto tendency = base_interaction->GetTendency();
        float lowest_influence = 1.0f;
        std::shared_ptr<Emotion> blocking_emotion(nullptr);
        for (auto &[type, value] : tendency->emotions)
        {
            if (value == 0)
            {
                continue;
            }
            auto emotion = chronicle_.GetLastEmotionOfType(base_interaction->tick_, base_interaction->GetOwner().lock()->id_, type).lock();
            float current_influence = emotion->GetValue() * value;
            if (current_influence < lowest_influence)
            {
                blocking_emotion = emotion;
                lowest_influence = current_influence;
            }
        }
        for (auto &consequence : base_interaction->GetConsequences())
        {
            goal_kernel = RecursivelyFindUnlikeliestConsequence(consequence.lock(), goal_kernel, depth);
        }
        const auto &connection = FindCausalConnection(base_interaction, goal_kernel);

        // x did this despite thing influenced negatively the most which she was because of this interaction. this lead to another unlikely event with this chain.
        std::string description = fmt::format("Something rare ({}) happened: {}", base_interaction->GetChance(), base_interaction->ToString());
        if (blocking_emotion)
        {
            description += fmt::format("\n\tThis happened despite this thing: {}", blocking_emotion->ToString());
        }
        if (goal_kernel)
        {
            description += fmt::format("\n\t\tThis somehow led to this unlikely ({}) thing: {} (during tick {})", goal_kernel->GetChance(), goal_kernel->ToString(), goal_kernel->tick_);
        }
        if (connection.size() > 2)
        {
            description += "\n\t\t\tHow did this happen? Well:";
            description += fmt::format("\n\t\t\tWe started with {}", connection.front()->ToString());
            for (size_t i = 1; i < connection.size() - 1; ++i)
            {
                description += fmt::format("\n\t\t\tThen {}", connection[i]->ToString());
            }
            description += fmt::format("\n\t\t\tAfter all that we ended with {}", connection.back()->ToString());
        }
        return description;
    }
} // namespace tattletale
