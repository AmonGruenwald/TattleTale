#include "shared/tattletalecore.hpp"
#include "shared/kernels/interactions/interaction.hpp"
#include <fmt/format.h>
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
        std::vector<std::weak_ptr<Kernel>> reasons,
        std::vector<std::weak_ptr<Actor>> participants)
        : Kernel(prototype->name, id, tick, participants[0], reasons),
          prototype_(prototype),
          requirement_(requirement),
          tendency_(tendency),
          chance_(chance),
          participants_(participants){};

    void Interaction::Apply()
    {
        std::weak_ptr<Interaction> self = std::static_pointer_cast<Interaction>(shared_from_this());
        std::vector<std::weak_ptr<Kernel>> reasons{self};
        for (size_t i = 0; i < participants_.size(); ++i)
        {
            participants_.at(i).lock()->ApplyWealthChange(reasons, tick_, prototype_->wealth_effects[i]);
            for (auto &[type, value] : prototype_->emotion_effects[i])
            {
                participants_.at(i).lock()->ApplyEmotionChange(reasons, tick_, type, value);
            }
            for (auto &[other, change] : prototype_->relationship_effects[i])
            {

                participants_.at(i).lock()->ApplyRelationshipChange(reasons, tick_, participants_[other].lock()->id_, change);
            }
        }
    }

    std::string Interaction::ToString()
    {
        fmt::format_args::format_arg args[12];
        int count = 0;
        for (auto &participant : participants_)
        {
            args[count++] = fmt::detail::make_arg<fmt::format_context>(participant.lock()->name_);
        }
        auto description = fmt::vformat(prototype_->description, fmt::format_args(args, count));
        return description;
    }

    const std::shared_ptr<InteractionPrototype> Interaction::GetPrototype() const
    {
        return prototype_;
    }
    const std::vector<std::weak_ptr<Actor>> &Interaction::GetParticipants() const
    {
        return participants_;
    }

    const float Interaction::GetChance() const
    {
        return chance_;
    }
} // namespace tattletale