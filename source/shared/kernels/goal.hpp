#ifndef TALE_GOAL_H
#define TALE_GOAL_H

#include "shared/kernels/kernel.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    /**
     * @brief Types of Goals an Actor can possess.
     */
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
     */
    class Goal : public Kernel
    {
    public:
        /**
         * @brief Gets the default description for the Goal.
         *
         * This description forms a standalone sentence. E.g.: "John Doe wants to become incredibly rich."
         * @return The default description.
         */
        std::string GetDefaultDescription() const override;
        /**
         * @brief Gets a passive description for the Goal.
         *
         * This description forms a passive sentence fragment that can be incorporated into a narrative.
         * E.g.: "wanting to become incredibly rich"
         * @return The passive description.
         */
        std::string GetPassiveDescription() const override;
        /**
         * @brief Gets an active description for the Goal.
         *
         * This description forms an active sentence fragment that can be incorporated into a narrative.
         * E.g.: "want to become incredibly rich"
         * @return The passive description.
         */
        std::string GetActiveDescription() const override;
        /**
         * @brief Converts a string to an GoalType.
         *
         * Uses the same string values formater returns.
         * So valid strings are: "wealth" "acceptance", "relationship", "hedonism" and "power".
         * Everything else just return GoalType::kLast. (But an assert will crash the Application in Debug mode.)
         *
         * @param goal_string The string we want to convert.
         * @return The corresponding GoalType value.
         */
        static GoalType StringToGoalType(std::string goal_string);

        /**
         * @brief The GoalType of this Goal.
         */
        const GoalType type_;

    private:
        /**
         * @brief Constructor, also initializing base Kernel class and type_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param type What GoalType this Goal is of.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Goal was created.
         * @param owner The Actor which owns this Goal.
         * @param reasons What other \link Kernel Kernels \endlink  led to this Goal.
         */
        Goal(GoalType type, size_t id, size_t tick, Actor *owner, std::vector<Kernel *> reasons);
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
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