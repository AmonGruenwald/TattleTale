#ifndef TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H
#define TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H

#include <fmt/format.h>
#include <fmt/core.h>
#include "shared/kernels/kernel.hpp"

namespace tattletale
{
    struct Verb
    {
        std::string default_verb;
        std::string passive_verb;
        std::string active_verb;
        Verb(std::string default_verb,
             std::string passive_verb,
             std::string active_verb) : default_verb(default_verb),
                                        passive_verb(passive_verb),
                                        active_verb(active_verb) {}
    };

    /**
     * @brief Represents an abstract Resource an Actor has.
     */
    class Resource : public Kernel
    {
    public:
        /**
         * @brief Getter for the value of the Resource.
         *
         * @return The value.
         */
        float GetValue() const;
        std::string GetDefaultDescription() const override;
        std::string GetPassiveDescription() const override;
        std::string GetActiveDescription() const override;

    protected:
        /**
         * @brief Constructor initializing base Kernel class and value_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param name The name of this Resource. Should describe what this Resource is tracking.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Resource was created.
         * @param owner The Actor which owns this Resource.
         * @param reasons What other \link Kernel Kernels \endlink  led to this Resource and its value.
         * @param value Value of the Resource between -1.0 and 1.0.
         */
        Resource(
            std::string name,
            std::string positive_name_variant,
            std::string negative_name_variant,
            size_t id,
            size_t tick,
            std::weak_ptr<Actor> owner,
            std::vector<std::weak_ptr<Kernel>> reasons,
            float value,
            KernelType type = KernelType::kResource,
            Verb verb = Verb("was", "beeing", "are"));
        virtual std::string GetAdjective() const;
        std::string GetNameVariant() const;
        /**
         * @brief The value of this Resource.
         */
        float value_;
        Verb verb_;
        const std::string positive_name_variant_;
        const std::string negative_name_variant_;
        friend class Chronicle;
    };
} // namespace tattletale
template <>
struct fmt::formatter<tattletale::Verb>
{
    // d - default, a - active, p - passive
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'd' || *it == 'a' || *it == 'p'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(const tattletale::Verb &verb, FormatContext &ctx) -> decltype(ctx.out())
    {
        if (presentation == 'p')
        {
            return format_to(ctx.out(), "{}", verb.passive_verb);
        }
        else if (presentation == 'a')
        {
            return format_to(ctx.out(), "{}", verb.active_verb);
        }

        return format_to(ctx.out(), "{}", verb.default_verb);
    }
};
#endif // TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H