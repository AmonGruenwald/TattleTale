#ifndef TALE_RELATIONSHIP_H
#define TALE_RELATIONSHIP_H

#include "tale/kernels/kernel.hpp"

namespace tale
{
    /**
     * @brief Represents a Relationship an Actor has with another Actor
     *
     */
    class Relationship : public Kernel
    {
    public:
        Relationship(std::string name, std::vector<std::weak_ptr<Kernel>> reasons);
        std::string ToString();
    };

} // namespace tale
#endif // TALE_RELATIONSHIP_H