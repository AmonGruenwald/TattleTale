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
            description = fmt::format("not having a goal", *owner_);
            break;
        case GoalType::kWealth:
            description = fmt::format("wanting to become incredibly rich", *owner_);
            break;
        case GoalType::kAcceptance:
            description = fmt::format("wanting to be fully accepted by their peers", *owner_);
            break;
        case GoalType::kRelationship:
            description = fmt::format("wanting to have a loving relationship", *owner_);
            break;
        case GoalType::kHedonism:
            description = fmt::format("only living for pleasure", *owner_);
            break;
        case GoalType::kPower:
            description = fmt::format("wanting to be the most powerful person in school", *owner_);
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
            description = fmt::format("do not have a goal", *owner_);
            break;
        case GoalType::kWealth:
            description = fmt::format("want to become incredibly rich", *owner_);
            break;
        case GoalType::kAcceptance:
            description = fmt::format("want to be fully accepted by their peers", *owner_);
            break;
        case GoalType::kRelationship:
            description = fmt::format("want to have a loving relationship", *owner_);
            break;
        case GoalType::kHedonism:
            description = fmt::format("only live for pleasure", *owner_);
            break;
        case GoalType::kPower:
            description = fmt::format("want to be the most powerful person in school", *owner_);
            break;
        }
        return description;
    }
} // namespace tattletale
