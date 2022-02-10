#ifndef TALE_COURSE_H
#define TALE_COURSE_H

#include "tale/globals/random.hpp"
#include "tale/actor.hpp"
#include <string>
#include <memory>

namespace tale
{
    /**
     * @brief Course managing actors
     */
    class Course
    {
    public:
        const std::string name_;
        const size_t id_;
        Course(Random &random, size_t id, std::string name_);
        bool AllSlotsFilled();
        int AddToRandomEmptySlot(std::vector<std::weak_ptr<Actor>> actors);

    private:
        std::vector<std::vector<std::weak_ptr<Actor>>> slots_;
        Random &random_;
    };

} // namespace tale
#endif // TALE_COURSE_H