#include "shared/kernels/goal.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include <iostream>
#include <assert.h>

namespace tattletale
{
    Goal::Goal(GoalType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons)
        : type_(type), Kernel(GoalTypeToString(type), id, tick, owner, reasons, KernelType::kGoal){};

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
        return GoalType::kNone;
    }
    std::string Goal::GoalTypeToString(GoalType goal_type)
    {
        switch (goal_type)
        {
        case GoalType::kNone:
            return "none";
            break;
        case GoalType::kLast:
            TATTLETALE_ERROR_PRINT(true, "Invalid Goal type was passed");
            return "last";
            break;
        case GoalType::kWealth:
            return "wealth";
            break;
        case GoalType::kAcceptance:
            return "acceptance";
            break;
        case GoalType::kRelationship:
            return "relationship";
            break;
        case GoalType::kHedonism:
            return "hedonism";
            break;
        case GoalType::kPower:
            return "power";
            break;
        }
        return "none";
    }

    std::string Goal::GoalTypeToDescription(GoalType goal_type) const
    {
        switch (goal_type)
        {
        case GoalType::kNone:
            return fmt::format("{} does not have a goal.", *owner_.lock());
            break;
        case GoalType::kLast:
            TATTLETALE_ERROR_PRINT(true, "Invalid Goal type was passed");
            return fmt::format("{} does not have a goal.", *owner_.lock());
            break;
        case GoalType::kWealth:
            return fmt::format("{} want to become incredibly rich.", *owner_.lock());
            break;
        case GoalType::kAcceptance:
            return fmt::format("{} wants to be fully accepted by their peers.", *owner_.lock());
            break;
        case GoalType::kRelationship:
            return fmt::format("{} wants to have a loving relationship.", *owner_.lock());
            break;
        case GoalType::kHedonism:
            return fmt::format("{} only lives for pleasure.", *owner_.lock());
            break;
        case GoalType::kPower:
            return fmt::format("{} wants to be the most powerful person in school.", *owner_.lock());
            break;
        }
        return "none";
    }
    std::string Goal::GetDefaultDescription() const
    {
        return GoalTypeToDescription(type_);
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
