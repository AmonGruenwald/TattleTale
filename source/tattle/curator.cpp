#include "tattle/curator.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include <math.h>
#include <map>
#include "shared/tattletalecore.hpp"

namespace tattletale
{
    Curator::Curator(const Chronicle &chronicle, const Setting &setting) : chronicle_(chronicle), setting_(setting) {}

    Kernel *Curator::RecursivelyFindUnlikeliestReason(Kernel *to_check, Kernel *current_best)
    {
        if (to_check->GetChance() < current_best->GetChance())
        {
            current_best = to_check;
        }
        auto &reasons = to_check->GetReasons();
        for (auto &reason : reasons)
        {
            current_best = RecursivelyFindUnlikeliestReason(reason, current_best);
        }
        return current_best;
    }
    Kernel *Curator::RecursivelyFindUnlikeliestConsequence(Kernel *to_check, Kernel *current_best, size_t depth)
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
                current_best = RecursivelyFindUnlikeliestConsequence(consequence, current_best, depth - 1);
            }
        }
        return current_best;
    }

    Resource *Curator::FindBlockingResource(Interaction *interaction) const
    {
        auto tendency = interaction->GetTendency();
        float lowest_influence = 1.0f;
        Resource *blocking_resource = nullptr;
        for (int type_index = 0; type_index < static_cast<int>(EmotionType::kLast); ++type_index)
        {
            float value = tendency->emotions[type_index];
            EmotionType type = static_cast<EmotionType>(type_index);
            if (value == 0)
            {
                continue;
            }
            auto last_emotion = chronicle_.GetLastEmotionOfType(interaction->tick_, interaction->GetOwner()->id_, type);
            float current_influence = last_emotion->GetValue() * value;
            if (current_influence < lowest_influence)
            {
                blocking_resource = last_emotion;
                lowest_influence = current_influence;
            }
        }
        if (tendency->wealth != 0)
        {
            auto last_wealth = chronicle_.GetLastWealth(interaction->tick_, interaction->GetOwner()->id_);
            float current_influence = last_wealth->GetValue() * tendency->wealth;
            if (current_influence < lowest_influence)
            {
                blocking_resource = last_wealth;
                lowest_influence = current_influence;
            }
        }
        return blocking_resource;
    }
    std::vector<Kernel *> Curator::FindCausalConnection(Kernel *start, Kernel *end) const
    {
        std::vector<Kernel *> causal_chain;
        RecursivelyFindCausalConnectionBackwards(end, start, causal_chain);
        return causal_chain;
    }

    bool Curator::RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const
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
            if (RecursivelyFindCausalConnectionBackwards(reason, start, out_causal_chain))
            {
                out_causal_chain.push_back(root);
                return true;
            }
        }
        return false;
    }
    std::string Curator::GetTimeDescription(Kernel *start, Kernel *end) const
    {
        size_t tick_distance = end->tick_ - start->tick_;
        if (tick_distance == 0)
        {
            return "At the same time";
        }
        if (tick_distance < (setting_.courses_per_day + 1) / 2)
        {
            return "Shortly after";
        }
        if (tick_distance < setting_.courses_per_day + 1)
        {
            return "Later that day";
        }
        size_t days = tick_distance / (setting_.courses_per_day + 1);
        if (days <= 1)
        {
            return "The following day";
        }
        if (days <= 7)
        {
            return "During the same week";
        }
        if (days <= 14)
        {
            return "About two week later";
        }
        if (days <= 31)
        {
            return "During the same month";
        }
        return "Quite a lot of time later";
    }

    std::string Curator::GetChanceDescription(float chance) const
    {
        if (chance < 0.1)
        {
            return "impossibly rare";
        }
        if (chance < 0.2)
        {
            return "rare";
        }
        if (chance < 0.3)
        {
            return "highly unlikely";
        }
        if (chance < 0.4)
        {
            return "unlikely";
        }
        if (chance < 0.5)
        {
            return "slightly unlikely";
        }
        if (chance < 0.6)
        {
            return "normal";
        }
        if (chance < 0.7)
        {
            return "somewhat common";
        }
        if (chance < 0.8)
        {
            return "common";
        }
        if (chance < 0.9)
        {
            return "very common";
        }
        return "completely banal";
    }

    std::string Curator::GetResourceReasonDescription(Resource *resource) const
    {
        float value = abs(resource->GetValue());
        if (value > 0.9f)
        {
            return fmt::format("resigning to their fate of {:p}, they just had to", *resource);
        }
        if (value > 0.7f)
        {
            return fmt::format("{:p} compelled them to", *resource);
        }
        if (value > 0.5f)
        {
            return fmt::format("them {:p} led them to", *resource);
        }
        if (value > 0.4f)
        {
            return fmt::format("because they were {:p} they had a feeling that they needed to", *resource);
        }
        if (value > 0.2f)
        {
            return fmt::format("{:p} gave them a slight excuse to", *resource);
        }
        return fmt::format("{:p} gave them a tiny nudge to", *resource);
    }
    std::string Curator::Curate()
    {
        TATTLETALE_DEBUG_PRINT("START CURATION");
        return RarityCuration();
    }
    std::string Curator::RarityCuration()
    {
        size_t depth = 5;
        size_t base_interaction_tick_distance_to_end = 10;
        size_t tick_cutoff = chronicle_.GetLastTick();
        tick_cutoff = tick_cutoff > depth ? tick_cutoff - base_interaction_tick_distance_to_end : tick_cutoff;
        auto base_interaction = chronicle_.FindUnlikeliestInteraction(tick_cutoff);
        if (!base_interaction)
        {
            return "Rarity Curation failed. No Interactions were created.";
        }
        auto normal_interaction = chronicle_.FindMostOccuringInteractionPrototypeForActor(base_interaction->GetOwner()->id_);
        auto blocking_resource = FindBlockingResource(base_interaction);

        Kernel *end_kernel = nullptr;
        for (auto &consequence : base_interaction->GetConsequences())
        {
            end_kernel = RecursivelyFindUnlikeliestConsequence(consequence, end_kernel, depth);
        }

        const auto &connection = FindCausalConnection(base_interaction, end_kernel);

        std::string description = fmt::format("Something {} happened with {}!\n", GetChanceDescription(base_interaction->GetChance()), *base_interaction->GetOwner());

        if (normal_interaction)
        {
            description += fmt::format("Normally one would find *{:f}* *{:p}*", *base_interaction->GetOwner(), *normal_interaction);
            if (blocking_resource)
            {
                description += fmt::format(", but despite *{:p}*, this time *{}*.", *blocking_resource, *base_interaction);
            }
            else
            {
                description += fmt::format(", but this time *{}*.", *base_interaction);
            }
        }
        else if (blocking_resource)
        {
            description += fmt::format("Despite *{:f}* *{:p}* *{}*", *blocking_resource->GetOwner(), *blocking_resource, *base_interaction);
        }
        else
        {
            description += fmt::format("*{}*.", *base_interaction);
        }

        if (end_kernel)
        {
            description += fmt::format("\nThis somehow led to another event that was *{}*. *{}*.", GetChanceDescription(end_kernel->GetChance()), *end_kernel);
        }
        if (connection.size() > 2)
        {
            description += "\n\nHow did it get to this?";
            std::string time_description = "Well, as already stated the story starts when ";
            std::map<size_t, std::vector<Resource *>> last_resource_values;
            for (size_t i = 0; i < connection.size() - 1; ++i)
            {
                auto owner = connection[i]->GetOwner();
                if (!last_resource_values.count(owner->id_))
                {
                    last_resource_values.insert({owner->id_, std::vector<Resource *>(static_cast<size_t>(EmotionType::kLast), nullptr)});
                }
                if (connection[i]->type_ == KernelType::kEmotion || connection[i]->type_ == KernelType::kResource)
                {
                    size_t type_index = last_resource_values[owner->id_].size() - 1;
                    if (connection[i]->type_ == KernelType::kEmotion)
                    {
                        auto emotion = dynamic_cast<Emotion *>(connection[i]);
                        size_t type_index = static_cast<size_t>(emotion->GetType());
                    }
                    auto resource = dynamic_cast<Resource *>(connection[i]);
                    if (last_resource_values[owner->id_][type_index])
                    {
                        float last_value = last_resource_values[owner->id_][type_index]->GetValue();
                        if (signbit(last_value) == signbit(resource->GetValue()))
                        {
                            continue;
                        }
                    }
                    last_resource_values[owner->id_][type_index] = resource;
                }
                if (connection[i]->type_ == KernelType::kEmotion)
                {
                    description += fmt::format(" which made *{:f}* *{:a}*", *owner, *connection[i]);
                }
                else if (connection[i]->type_ == KernelType::kResource)
                {
                    description += fmt::format(" which made *{:f}* *{:a}*", *owner, *connection[i]);
                }
                else
                {
                    std::string reason_description = "";
                    std::string event_description = fmt::format("{}", *connection[i]);
                    if (i > 0)
                    {
                        time_description = fmt::format("*{}*", GetTimeDescription(connection[i - 1], connection[i]));
                        if (connection[i - 1]->type_ == KernelType::kResource || connection[i - 1]->type_ == KernelType::kEmotion)
                        {
                            auto reason = dynamic_cast<Resource *>(connection[i - 1]);
                            reason_description = fmt::format(", *{}*", GetResourceReasonDescription(reason));
                        }
                        description += ".";
                        event_description = reason_description == "" ? fmt::format("*{}*", *connection[i]) : fmt::format("*{:a}*", *connection[i]);
                    }
                    description += fmt::format("\n{}{} {}", time_description, reason_description, event_description);
                }
            }
            description += fmt::format("\nAnd after all that this unlikely chain of events ended when *{}*", *connection.back());
            // TODO: Add reason for last inteaction
            // TODO: deal with relationships
        }
        return description;
    }
} // namespace tattletale
