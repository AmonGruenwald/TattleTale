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
        std::string ToString();

    private:
        Goal(std::string name, size_t id, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        friend class Chronicle;
    };

} // namespace tale
#endif // TALE_GOAL_H