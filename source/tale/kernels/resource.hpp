#ifndef TALE_RESOURCE_H
#define TALE_RESOURCE_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    /**
     * @brief Represents the abstract Resources an Actor has.
     *
     */
    class Resource : public Kernel
    {
    public:
        Resource(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons, float value);
        float GetValue() const;
        std::string ToString();

    private:
        float value_;
    };

} // namespace tale
#endif // TALE_RESOURCE_H