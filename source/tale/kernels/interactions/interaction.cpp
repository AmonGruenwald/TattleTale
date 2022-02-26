#include "tale/talecore.hpp"
#include "tale/kernels/interactions/interaction.hpp"
#include <fmt/core.h>
#include <iostream>
#include "tale/actor.hpp"

namespace tale
{
    Interaction::Interaction(
        const InteractionPrototype &prototype,
        size_t id,
        size_t tick,
        std::vector<std::weak_ptr<Kernel>> reasons,
        std::vector<std::weak_ptr<Actor>> participants)
        : Kernel(prototype.name, id, tick, reasons),
          prototype_(prototype),
          participants_(participants){};

    void Interaction::Apply()
    {
        std::weak_ptr<Interaction> self = std::static_pointer_cast<Interaction>(shared_from_this());
        std::vector<std::weak_ptr<Kernel>> reasons{self};
        for (size_t i = 0; i < participants_.size(); ++i)
        {
            participants_.at(i).lock()->ApplyWealthChange(reasons, tick_, prototype_.wealth_effects[i]);
            for (auto &[type, value] : prototype_.emotion_effects[i])
            {
                participants_.at(i).lock()->ApplyEmotionChange(reasons, tick_, type, value);
            }
            for (auto &[other, change] : prototype_.relationship_effects[i])
            {
                for (auto &[type, value] : change)
                {
                    participants_.at(i).lock()->ApplyRelationshipChange(reasons, tick_, participants_[other].lock()->id_, type, value);
                }
            }
        }
    }

    std::string Interaction::ToString()
    {
        std::string description = "";
        description += fmt::format("{} does {}", participants_[0].lock()->name_, name_);
        for (size_t i = 1; i < participants_.size(); ++i)
        {
            if (i == 1)
            {
                description += " with ";
            }
            else
            {
                description += " and ";
            }
            description += fmt::format("{}", participants_[i].lock()->name_);
        }
        return description;
    }

    const InteractionPrototype &Interaction::GetPrototype() const
    {
        return prototype_;
    }
    const std::vector<std::weak_ptr<Actor>> &Interaction::GetParticipants() const
    {
        return participants_;
    }
} // namespace tale