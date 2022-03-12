#ifndef TALE_GOAL_H
#define TALE_GOAL_H

#include "shared/kernels/kernel.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    enum class GoalType
    {
        kNone,
        kWealth,
        kAcceptance,
        kRelationship,
        kHedonism,
        kPower,
        kLast
    };
    /**
     * @brief Represents the Goal an Actor strives to fulfill.
     *
     */
    class Goal : public Kernel
    {
    public:
        std::string ToString();
        static GoalType GetRandomGoalType(Random &random);
        static GoalType StringToGoalType(std::string goal_string);
        static std::string GoalTypeToString(GoalType goal_type);
        std::string GoalTypeToDescription(GoalType goal_type);

        const GoalType type_;

    private:
        Goal(GoalType type, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);
        friend class Chronicle;
    };

} // namespace tattletale
#endif // TALE_GOAL_H