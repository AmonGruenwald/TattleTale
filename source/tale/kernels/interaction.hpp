#ifndef TALE_INTERACTION_H
#define TALE_INTERACTION_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    /**
     * @brief Represents an Interaction an Actor had with another Actor
     *
     */
    class Interaction : public Kernel
    {
    public:
        Interaction(std::string name, std::vector<std::weak_ptr<Kernel>> reasons);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_INTERACTION_H