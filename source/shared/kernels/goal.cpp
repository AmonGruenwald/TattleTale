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
        std::string description = "No default description.";
        switch (type_)
        {
        case GoalType::kLast:
            description = fmt::format("{} does not have a goal.", *owner_);
            break;
        case GoalType::kWealth:
            description = fmt::format("{} want to become incredibly rich.", *owner_);
            break;
        case GoalType::kAcceptance:
            description = fmt::format("{} wants to be fully accepted by their peers.", *owner_);
            break;
        case GoalType::kRelationship:
            description = fmt::format("{} wants to have a loving relationship.", *owner_);
            break;
        case GoalType::kHedonism:
            description = fmt::format("{} only lives for pleasure.", *owner_);
            break;
        case GoalType::kPower:
            description = fmt::format("{} wants to be the most powerful person in school.", *owner_);
            break;
        }
        return description;
    }
    std::string Goal::GetPassiveDescription() const
    {
        return name_;
    }
    std::string Goal::GetActiveDescription() const
    {
        return GetDefaultDescription();
    }
} // namespace tattletale
