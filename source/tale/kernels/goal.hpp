#ifndef TALE_GOAL_H
#define TALE_GOAL_H

#include "tale/kernels/kernel.hpp"
#include "tale/globals/random.hpp"

namespace tale
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

        const GoalType type_;

    private:
        Goal(GoalType type, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        friend class Chronicle;
    };

} // namespace tale
#endif // TALE_GOAL_H