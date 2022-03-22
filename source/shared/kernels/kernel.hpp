#ifndef TALE_KERNEL_H
#define TALE_KERNEL_H

#include <string>
#include <vector>
#include <memory>
#include <fmt/core.h>
#include <fmt/format.h>

namespace tattletale
{
    enum class KernelType
    {
        kNone,
        kResource,
        kEmotion,
        kRelationship,
        kInteraction,
        kTrait,
        kGoal,
        kLast
    };
    class Actor;
    /**
     * @brief Represents a part of the simulation, like an interaction, or an emotional goal.
     * To build up causality parts of the story have to reference each other. Because of that every
     * part of the story hast to be made up of a shared interface, which is what this class is. For
     * said causality everything needs to have possible reasons and consequences. In addition temporal
     * tracking is supported though increasing numbers for each Kernel and storing on which tick the
     * Kernel was created.
     */
    class Kernel
    {
    public:
        std::string name_;
        size_t id_;
        size_t tick_;
        KernelType type_;
        virtual ~Kernel();

        void AddConsequence(Kernel *consequence);
        const std::vector<Kernel *> &GetConsequences() const;
        virtual const std::vector<Kernel *> &GetReasons() const;
        virtual std::string GetDefaultDescription() const = 0;
        virtual std::string GetDetailedDescription() const;
        virtual std::string GetPassiveDescription() const = 0;
        virtual std::string GetActiveDescription() const = 0;
        virtual float GetChance() const;
        Actor *GetOwner() const;

    protected:
        /**
         * @brief The Actor owning this Kernel.
         */
        std::vector<Kernel *> consequences_;
        std::vector<Kernel *> reasons_;
        Actor *owner_;
        Kernel(std::string name, size_t id, size_t tick, Actor *owner, std::vector<Kernel *> reasons, KernelType type);
    };

} // namespace tattletale
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::Kernel, T>::value, char>> : fmt::formatter<std::string>
{
    // d - default, a - active, p - passive, n - name, o - detailed
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'a' || *it == 'p' || *it == 'n' || *it == 'd' || *it == 'o'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(tattletale::Kernel &kernel, FormatContext &ctx) -> decltype(ctx.out())
    {
        if (presentation == 'a')
        {
            return fmt::formatter<std::string>::format(kernel.GetActiveDescription(), ctx);
        }
        else if (presentation == 'p')
        {
            return fmt::formatter<std::string>::format(kernel.GetPassiveDescription(), ctx);
        }
        else if (presentation == 'n')
        {
            return fmt::formatter<std::string>::format(kernel.name_, ctx);
        }
        else if (presentation == 'o')
        {
            return fmt::formatter<std::string>::format(kernel.GetDetailedDescription(), ctx);
        }
        return fmt::formatter<std::string>::format(kernel.GetDefaultDescription(), ctx);
    }
};
template <>
struct fmt::formatter<tattletale::KernelType> : formatter<string_view>
{
    std::string kernel_type_names[8] = {
        "none",
        "resource",
        "emotion",
        "relationship",
        "interaction",
        "trait",
        "goal",
        "last",
    };
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(tattletale::KernelType type, FormatContext &ctx)
    {
        size_t enum_index = static_cast<size_t>(type);
        string_view name = kernel_type_names[enum_index];
        return formatter<string_view>::format(name, ctx);
    }
};

#endif // TALE_KERNEL_H