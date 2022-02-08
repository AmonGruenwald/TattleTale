#ifndef TALE_GOAL_H
#define TALE_GOAL_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    /**
     * @brief Represents the Goal an Actor strives to fulfill.
     *
     */
    class Goal : public Kernel
    {
    public:
        Goal(std::string name, std::vector<std::weak_ptr<Kernel>> reasons);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_GOAL_H