#ifndef TALE_KERNEL_H
#define TALE_KERNEL_H

#include <string>
#include <vector>
#include <memory>

namespace tale
{
    class Actor;
    /**
     * @brief Represents a part of the simulation, like an interaction, or an emotional goal.
     * To build up causality parts of the story have to reference each other. Because of that every
     * part of the story hast to be made up of a shared interface, which is what this class is. For
     * said causality everything needs to have possible reasons and consequences. In addition temporal
     * tracking is supported though increasing numbers for each Kernel and storing on which tick the
     * Kernel was created.
     */
    class Kernel : public std::enable_shared_from_this<Kernel>
    {
    public:
        std::string name_;
        std::vector<std::weak_ptr<Kernel>> reasons_;
        std::vector<std::weak_ptr<Kernel>> consequences_;
        size_t id_;
        size_t tick_;

        void AddConsequence(std::weak_ptr<Kernel> consequence);
        virtual std::string ToString() = 0;

    protected:
        /**
         * @brief The Actor owning this Kernel.
         */
        std::weak_ptr<Actor> owner_;
        Kernel(std::string name, size_t id, size_t tick, std::weak_ptr<Actor> owner, std::vector<std::weak_ptr<Kernel>> reasons);
    };

} // namespace tale
#endif // TALE_KERNEL_H