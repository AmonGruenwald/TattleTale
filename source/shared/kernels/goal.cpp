#include "shared/kernels/goal.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include <iostream>
#include <assert.h>

namespace tattletale
{
    Goal::Goal(GoalType type, size_t id, size_t tick, Actor *owner, std::vector<Kernel *> reasons)
        : type_(type), Kernel(fmt::format("{}", type), id, tick, owner, reasons, KernelType::kGoal){};

    GoalType Goal::GetRandomGoalType(Random &random)
    {
        uint32_t enum_count = static_cast<size_t>(GoalType::kLast);
        uint32_t random_enum = random.GetUInt(1, enum_count - 1);
        return static_cast<GoalType>(random_enum);
    }
    GoalType Goal::StringToGoalType(std::string goal_string)
    {
        if (goal_string == "wealth")
            return GoalType::kWealth;
        if (goal_string == "acceptance")
            return GoalType::kAcceptance;
        if (goal_string == "relationship")
            return GoalType::kRelationship;
        if (goal_string == "hedonism")
            return GoalType::kHedonism;
        if (goal_string == "power")
            return GoalType::kPower;
        return GoalType::kLast;
    }

    std::string Goal::GetDefaultDescription() const
    {
        std::string description = "No default description";
        switch (type_)
        {
        case GoalType::kLast:
            description = fmt::format("{} did not have a goal", *owner_);
            break;
        case GoalType::kWealth:
            description = fmt::format("{} wanted to become incredibly rich", *owner_);
            break;
        case GoalType::kAcceptance:
            description = fmt::format("{} wanted to be fully accepted by their peers", *owner_);
            break;
        case GoalType::kRelationship:
            description = fmt::format("{} wanted to have a loving relationship", *owner_);
            break;
        case GoalType::kHedonism:
            description = fmt::format("{} only lived for pleasure", *owner_);
            break;
        case GoalType::kPower:
            description = fmt::format("{} wanted to be the most powerful person in school", *owner_);
            break;
        }
        return description;
    }
    std::string Goal::GetPassiveDescription() const
    {
        std::string description = "No passive description";
        switch (type_)
        {
        case GoalType::kLast:
            description = fmt::format("not having a goal");
            break;
        case GoalType::kWealth:
            description = fmt::format("wanting to become incredibly rich");
            break;
        case GoalType::kAcceptance:
            description = fmt::format("wanting to be fully accepted by their peers");
            break;
        case GoalType::kRelationship:
            description = fmt::format("wanting to have a loving relationship");
            break;
        case GoalType::kHedonism:
            description = fmt::format("only living for pleasure");
            break;
        case GoalType::kPower:
            description = fmt::format("wanting to be the most powerful person in school");
            break;
        }
        return description;
    }
    std::string Goal::GetActiveDescription() const
    {
        std::string description = "No active description";
        switch (type_)
        {
        case GoalType::kLast:
            description = fmt::format("do not have a goal");
            break;
        case GoalType::kWealth:
            description = fmt::format("want to become incredibly rich");
            break;
        case GoalType::kAcceptance:
            description = fmt::format("want to be fully accepted by their peers");
            break;
        case GoalType::kRelationship:
            description = fmt::format("want to have a loving relationship");
            break;
        case GoalType::kHedonism:
            description = fmt::format("only live for pleasure");
            break;
        case GoalType::kPower:
            description = fmt::format("want to be the most powerful person in school");
            break;
        }
        return description;
    }

    float Goal::CalculateChanceInfluence(const Interaction *interaction) const
    {
        float influence = 0;
        auto &relationship_effects = interaction->GetPrototype()->relationship_effects;
        switch (type_)
        {
        case GoalType::kWealth:
            influence = interaction->GetPrototype()->wealth_effects[0];
            break;
        case GoalType::kAcceptance:
            // skip first as we want the values of other people to this actor
            for (size_t i = 1; i < relationship_effects.size(); ++i)
            {
                if (relationship_effects[i].count(0))
                {
                    influence += relationship_effects[i].at(0).at(static_cast<int>(RelationshipType::kFriendship));
                }
            }
            break;
        case GoalType::kRelationship:
            // skip first as we want the values of other people to this actor
            for (size_t i = 1; i < relationship_effects.size(); ++i)
            {
                // do both sides of the relationship change
                if (relationship_effects[i].count(0) && relationship_effects[0].count(i))
                {
                    influence += relationship_effects[i].at(0).at(static_cast<int>(RelationshipType::kLove));
                    influence += relationship_effects[0].at(1).at(static_cast<int>(RelationshipType::kLove));
                }
            }
            break;
        case GoalType::kHedonism:
            influence = interaction->GetPrototype()->emotion_effects[0][static_cast<int>(EmotionType::kSatisfied)];
            break;
        case GoalType::kPower:
            // skip first as we want the values of other people to this actor
            for (const auto &[other_actor, effect] : relationship_effects[0])
            {
                // using minus here because the actor wants a negative value
                influence -= effect.at(static_cast<int>(RelationshipType::kProtective));
            }
            break;
        case GoalType::kLast:
            TATTLETALE_ERROR_PRINT(true, "Trying to apply invalid goal type");
            break;
        }
        influence = std::clamp(influence, -1.0f, 1.0f);
        return influence;
    }
    bool Goal::IsSameSpecificType(Kernel *other) const
    {
        if (!IsSameKernelType(other))
        {
            return false;
        }
        return (dynamic_cast<Goal *>(other)->type_ == type_);
    }
} // namespace tattletale
