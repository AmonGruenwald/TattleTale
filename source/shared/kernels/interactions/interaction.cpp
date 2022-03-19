#include "shared/tattletalecore.hpp"
#include "shared/kernels/interactions/interaction.hpp"
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/args.h>
#include <iostream>
#include "shared/actor.hpp"

namespace tattletale
{
    Interaction::Interaction(
        const std::shared_ptr<InteractionPrototype> prototype,
        const std::shared_ptr<InteractionRequirement> requirement,
        const std::shared_ptr<InteractionTendency> tendency,
        float chance,
        size_t id,
        size_t tick,
        std::vector<Kernel *> reasons,
        std::vector<Actor *> participants)
        : Kernel(prototype->name, id, tick, participants[0], reasons, KernelType::kInteraction),
          prototype_(prototype),
          requirement_(requirement),
          tendency_(tendency),
          chance_(chance),
          participants_(participants){};

    void Interaction::Apply()
    {
        std::vector<Kernel *> reasons{this};
        for (size_t i = 0; i < participants_.size(); ++i)
        {
            participants_.at(i)->ApplyWealthChange(reasons, tick_, prototype_->wealth_effects[i]);
            for (auto &[type, value] : prototype_->emotion_effects[i])
            {
                participants_.at(i)->ApplyEmotionChange(reasons, tick_, type, value);
            }
            for (auto &[other, change] : prototype_->relationship_effects[i])
            {

                participants_.at(i)->ApplyRelationshipChange(reasons, tick_, participants_[other]->id_, change);
            }
        }
    }

    std::string Interaction::GetDefaultDescription() const
    {
        fmt::format_args::format_arg args[12];
        int count = 0;
        for (auto &participant : participants_)
        {
            args[count++] = fmt::detail::make_arg<fmt::format_context>(participant->name_);
        }
        auto description = fmt::vformat(prototype_->description, fmt::format_args(args, count));
        return description;
    }
    std::string Interaction::GetDetailedDescription() const
    {
        std::string description = fmt::format("{}", Kernel::GetDetailedDescription());

        if (participants_.size() == 2)
        {
            return fmt::format("{} with participant {}", description, *participants_[1]);
        }
        else if (participants_.size() >= 3)
        {
            description = fmt::format("{} with participants {}", description, *participants_[1]);

            for (size_t i = 2; i < participants_.size(); ++i)
            {
                description = fmt::format("{}, {}", description, *participants_[i]);
            }
        }
        return description;
    }
    std::string Interaction::GetPassiveDescription() const
    {
        return prototype_->passive_description;
    }
    std::string Interaction::GetActiveDescription() const
    {
        fmt::format_args::format_arg args[12];
        int count = 0;
        for (size_t i = 1; i < participants_.size(); ++i)
        {
            args[count++] = fmt::detail::make_arg<fmt::format_context>(participants_[i]->name_);
        }
        auto description = fmt::vformat(prototype_->active_description, fmt::format_args(args, count));
        return description;
    }

    const std::shared_ptr<InteractionPrototype> Interaction::GetPrototype() const
    {
        return prototype_;
    }
    const std::shared_ptr<InteractionTendency> Interaction::GetTendency() const
    {
        return tendency_;
    }
    const std::vector<Actor *> &Interaction::GetParticipants() const
    {
        return participants_;
    }

    float Interaction::GetChance() const
    {
        return chance_;
    }
} // namespace tattletale