#ifndef TALE_KERNEL_H
#define TALE_KERNEL_H

#include <string>
#include <vector>
#include <memory>
#include <fmt/core.h>
#include <fmt/format.h>

namespace tattletale
{
    /**
     * @brief The types of Kernel that can be created.
     */
    enum class KernelType
    {
        kNone,
        kResource,
        kEmotion,
        kRelationship,
        kInteraction,
        kGoal,
        kLast
    };
    class Actor;
    /**
     * @brief Represents a part of the simulation, like an interaction, or an emotional goal.
     *
     * To build up causality parts of the story have to reference each other. Because of that every
     * part of the story hast to be made up of a shared interface, which is what this class is. For
     * said causality everything needs to have possible reasons and consequences. In addition temporal
     * tracking is supported though increasing numbers for each Kernel and storing on which tick the
     * Kernel was created.
     */
    class Kernel
    {
    public:
        /**
         * @brief The name of this Kernel.
         */
        std::string name_;
        /**
         * @brief The id of this Kernel in the Chronicle.
         */
        size_t id_;
        /**
         * @brief The tick during which this Kernel was created.
         */
        size_t tick_;
        /**
         * @brief The KernelType of this Kernel.
         */
        KernelType type_;
        /**
         * @brief Virtual destructor doing nothing.
         */
        virtual ~Kernel();

        /**
         * @brief Adds another Kernel for which this Kernel was a reason for.
         *
         * @param consequence The other Kernel which was caused by this Kernel.
         */
        void AddConsequence(Kernel *consequence);
        /**
         * @brief Getter for the vector of Kernel this Kernel was the cause of.
         *
         * @return The vector of consequences.
         */
        const std::vector<Kernel *> &GetConsequences() const;
        /**
         * @brief Gets the \link Kernel Kernels \endlink that were the reason for the creation of this Kernel.
         *
         * @return The vector of reasons.
         */
        virtual const std::vector<Kernel *> &GetReasons() const;
        /**
         * @brief Abstract method to get the default description for this Kernel.
         *
         * This description should be a complete sentence, that can be used in a narration.
         *
         * @return The default description
         */
        virtual std::string GetDefaultDescription() const = 0;
        /**
         * @brief Gets a detailed description of the Kernel for debugging purposes.
         *
         * This description should not be used for curation at it is not written in a proper way.
         * It only gives an overview of the Kernel to print out during debugging.
         *
         * @return The detailed description
         */
        virtual std::string GetDetailedDescription() const;
        /**
         * @brief Abstract method to get a passive description for this Kernel.
         *
         * This description should be a sentence fragment that can be incorporated into a narrative.
         * It should fit for example into the following gap: "One often sees them __________"
         *
         * @return The passive description
         */
        virtual std::string GetPassiveDescription() const = 0;
        /**
         * @brief Abstract method to get an active description for this Kernel.
         *
         * This description should be a sentence fragment that can be incorporated into a narrative.
         * It should fit for example into the following gap: "This caused them to __________"
         *
         * @return The active description
         */
        virtual std::string GetActiveDescription() const = 0;
        /**
         * @brief Get the chance this Kernel had to be created.
         *
         * @return The chance.
         */
        virtual float GetChance() const;
        /**
         * @brief Get the Actor owning this Kernel.
         *
         * @return The owner.
         */
        Actor *GetOwner() const;
        /**
         * @brief Getter for the participants this Kernel uses.
         *
         * For most \link Kernel Kernels \endlink this just contains the owner.
         *
         * @return The participants.
         */
        virtual std::vector<Actor *> GetAllParticipants() const;
        /**
         * @brief Getter for the absolute interest score of this Kernel.
         *
         * This is zero for every Kernel other than \link Interaction Interactions \endlink.
         *
         * @return The score.
         */
        virtual size_t GetAbsoluteInterestScore() const;

    protected:
        /**
         * @brief The Actor owning this Kernel.
         */
        Actor *owner_;
        /**
         * @brief Stores all other \link Kernel Kernels \endlink that were caused by this Kernel.
         */
        std::vector<Kernel *> consequences_;
        /**
         * @brief Stores all \link Kernel Kernels \endlink that caused this Kernel to be created.
         */
        std::vector<Kernel *> reasons_;
        /**
         * @brief Protected Constructor should only be called by derived classes.
         *
         * @param name The name of this Kernel.
         * @param id The id of this Kernel in the Chronicle.
         * @param tick The tick during which this Kernel is created.
         * @param owner The Actor owning this Kernel.
         * @param reasons Other \link Kernel Kernels \endlink that caused this Kernel to be created.
         * @param type The KernelType of this Kernel.
         */
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
    std::string kernel_type_names[7] = {
        "none",
        "resource",
        "emotion",
        "relationship",
        "interaction",
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