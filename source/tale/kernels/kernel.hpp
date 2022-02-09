#ifndef TALE_KERNEL_H
#define TALE_KERNEL_H

#include <string>
#include <vector>
#include <memory>

namespace tale
{
    /**
     * @brief Represents a part of the simulation, like an interaction, or an emotional goal.
     * To build up causality parts of the story have to reference each other. Because of that every
     * part of the story hast to be made up of a shared interface, which is what this class is. For
     * said causality everything needs to have possible reasons and consequences. In addition temporal
     * tracking is supported though increasing numbers for each Kernel and storing on which tick the
     * Kernel was created.
     */
    class Kernel
    {
    public:
        static size_t current_number_;

        std::string name_;
        std::vector<std::weak_ptr<Kernel>> reasons_;
        std::vector<std::weak_ptr<Kernel>> consequences_;
        size_t number_;
        size_t tick_;

        Kernel(std::string name, size_t tick, std::vector<std::weak_ptr<Kernel>> reasons);
        void AddConsequence(std::weak_ptr<Kernel> consequence);
        virtual std::string ToString() = 0;

    protected:
        /**
         * @brief Default constructor should generally not be callable.
         *
         * It only exists because some Prototypes shoule be able to create Kernels without increasing
         * the number_ or giving reasons_.
         */
        Kernel();
    };

} // namespace tale
#endif // TALE_KERNEL_H