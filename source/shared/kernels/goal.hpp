#ifndef TALE_GOAL_H
#define TALE_GOAL_H

#include "shared/kernels/kernel.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    enum class GoalType
    {
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
        std::string GetDefaultDescription() const override;
        std::string GetPassiveDescription() const override;
        std::string GetActiveDescription() const override;
        static GoalType GetRandomGoalType(Random &random);
        static GoalType StringToGoalType(std::string goal_string);

        const GoalType type_;

    private:
        Goal(GoalType type, size_t id, size_t tick, Actor *, std::vector<Kernel *> reasons);
        friend class Chronicle;
    };

} // namespace tattletale`

template <>
struct fmt::formatter<tattletale::GoalType> : formatter<string_view>
{
    std::string goal_type_names[6] = {
        "wealth",
        "acceptance",
        "relationship",
        "hedonism",
        "power",
        "last"};
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(tattletale::GoalType type, FormatContext &ctx)
    {
        size_t enum_index = static_cast<size_t>(type);
        string_view name = goal_type_names[enum_index];
        return formatter<string_view>::format(name, ctx);
    }
};
#endif // TALE_GOAL_H