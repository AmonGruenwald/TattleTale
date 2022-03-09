#include "tale/kernels/goal.hpp"

#include <iostream>
#include <assert.h>

namespace tale
{
    Goal::Goal(GoalType type, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons)
        : type_(type), Kernel(GoalTypeToString(type), id, tick, reasons){};

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
        assert(false); // invalid string was passed
        return GoalType::kNone;
    }
    std::string Goal::GoalTypeToString(GoalType goal_type)
    {
        switch (goal_type)
        {
        case GoalType::kNone:
            assert(false); // invalid enum was passed
            return "none";
            break;
        case GoalType::kLast:
            assert(false); // invalid enum was passed
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
    std::string Goal::ToString()
    {
        return ("Has the goal of achieving " + name_ + ".");
    }
} // namespace tale
