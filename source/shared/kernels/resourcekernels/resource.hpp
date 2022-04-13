#ifndef TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H
#define TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H

#include <fmt/format.h>
#include <fmt/core.h>
#include "shared/kernels/kernel.hpp"

namespace tattletale
{
    /**
     * @brief Stores all necessary strings to complete different versions of descriptions for \link Resource Resources \endlink.
     */
    struct Verb
    {
        /**
         * @brief Verb used for default descriptions. Should fit into the sentence "John _____ something." in the past tense.
         */
        std::string default_verb;
        /**
         * @brief Verb used for passive descriptions. Should fit into the sentence "John was _____ something."
         */
        std::string passive_verb;
        /**
         * @brief Verb used for active descriptions. Should fit into the sentence "They _____ something." in the present tense.
         */
        std::string active_verb;
        /**
         * @brief Constructor filling in all verbs.
         */
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
         * @brief Virtual destructor, does not do anything for now.
         */
        virtual ~Resource();
        /**
         * @brief Getter for the value of the Resource.
         *
         * @return The value.
         */
        float GetValue() const;

        /**
         * @brief Gets the default description for the Resource.
         *
         * This description forms a standalone sentence. E.g.: "John Doe was quite wealthy."
         * @return The default description.
         */
        std::string GetDefaultDescription() const override;
        /**
         * @brief Gets a detailed description for the Resource.
         *
         * This is only used for debugging purposes and should not be incorporated into curation as it is not in the
         * form of a well structured sentence.
         * @return The detailed description.
         */
        std::string GetDetailedDescription() const override;
        /**
         * @brief Gets a passive description for the Resource.
         *
         * This description forms a passive sentence fragment that can be incorporated into a narrative.
         * E.g.: "beeing quite wealthy"
         * @return The passive description.
         */
        std::string GetPassiveDescription() const override;
        /**
         * @brief Gets an active description for the Resource.
         *
         * This description forms an active sentence fragment that can be incorporated into a narrative.
         * E.g.: "are quite wealthy"
         * @return The passive description.
         */
        std::string GetActiveDescription() const override;

        // TODO: documentation
        virtual float CalculateChanceInfluence(const Interaction *interaction) const override;
        // TODO: documentation
        virtual bool IsSameSpecificType(Kernel *other) const;

    protected:
        /**
         * @brief Constructor initializing base Kernel class and value_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param name The name of this Resource. Should describe what this Resource is tracking.
         * @param positive_name_variant The positive name variant of the resource. E.g. wealthy.
         * @param negative_name_variant The negative name variant of the resource. E.g. poor.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Resource was created.
         * @param owner The Actor which owns this Resource.
         * @param reasons What other \link Kernel Kernels \endlink  led to this Resource and its value.
         * @param value Value of the Resource between -1.0 and 1.0.
         * @param type The KernelType of this resource.
         * @param verb Verb struct holding all necessary strings to complete the descriptions.
         */
        Resource(
            std::string name,
            std::string positive_name_variant,
            std::string negative_name_variant,
            size_t id,
            size_t tick,
            Actor *owner,
            std::vector<Kernel *> reasons,
            float value,
            KernelType type = KernelType::kResource,
            Verb verb = Verb("was", "beeing", "are"));
        /**
         * @brief Getter to find an adjective describing the severity of this Resource.
         *
         * @return The appropriate adjective.
         */
        virtual std::string GetAdjective() const;
        /**
         * @brief Getter for the name variant depending on the value of the Resource. E.g "wealthy" if it is positive "poor" if it is negative.
         *
         * @return The appropriate name variant.
         */
        std::string GetNameVariant() const;
        /**
         * @brief The value of this Resource.
         */
        float value_;
        /**
         * @brief The Verb storing the strings nvecessary for description construction.
         */
        Verb verb_;
        /**
         * @brief Name variant when the value of this Resource is positive.
         */
        const std::string positive_name_variant_;
        /**
         * @brief Name variant when the value of this Resource is negative.
         */
        const std::string negative_name_variant_;
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
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