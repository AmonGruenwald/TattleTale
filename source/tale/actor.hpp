#ifndef TALE_ACTOR_H
#define TALE_ACTOR_H

#include <string>

namespace tale
{
    /**
     * @brief Represents one person in the simulation.
     * An actor has to act accordingly to their internal state. So this class needs to contain everything neccessary.
     */
    class Actor
    {
    public:
        Actor();
        std::string name_;
    };

} // namespace tale
#endif // TALE_ACTOR_H