#ifndef TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H
#define TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H

#include "tale/kernels/kernel.hpp"

namespace tale
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
        std::string ToString();

    protected:
        /**
         * @brief Constructor initializing base Kernel class and value_ member.
         *
         * This should only ever be called through the Chronicle.
         *
         * @param name The name of this Resource. Should describe what this Resource is tracking.
         * @param id The index this Kernel holds in the Chronicle.
         * @param tick In which tick this Resource was created.
         * @param reasons What other \link Kernel Kernels \endlink  led to this Resource and its value.
         * @param value Value of the Resource between -1.0 and 1.0.
         */
        Resource(std::string name, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value);

    private:
        /**
         * @brief The value of this Resource.
         */
        float value_;
        friend class Chronicle;
    };

} // namespace tale
#endif // TALE_KERNELS_RESOURCEKERNELS_RESOURCE_H