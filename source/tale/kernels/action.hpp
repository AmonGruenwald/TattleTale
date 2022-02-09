#ifndef TALE_ACTION_H
#define TALE_ACTION_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    /**
     * @brief Represents an Action an Actor took that did not involve other Actors.
     *
     */
    class Action : public Kernel
    {
    public:
        Action(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_ACTION_H