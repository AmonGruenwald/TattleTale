#ifndef TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H
#define TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H

#include "shared/kernels/kernel.hpp"

namespace tattletale
{
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
        /**
         * @brief Creates a string describing the Resource for easy printing.
         *
         * @return The description string.
         */
        virtual std::string ToString();
        std::string GetNameVariant();

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
        Resource(std::string name, std::string positive_name_variant, std::string negative_name_variant, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons, float value, KernelType type = KernelType::kResource);
        /**
         * @brief The value of this Resource.
         */
        float value_;
        const char *adjectives_[4] = {"slightly", "quite", "very", "completely"};
        const std::string positive_name_variant_;
        const std::string negative_name_variant_;
        friend class Chronicle;
    };

} // namespace tattletale
#endif // TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H