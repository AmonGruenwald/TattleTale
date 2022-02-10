#ifndef TALE_SCHOOL_H
#define TALE_SCHOOL_H

#include <memory>
#include <vector>
#include "tale/course.hpp"
#include "tale/actor.hpp"

namespace tale
{
    /**
     * @brief School managing actors and courses
     */
    class School
    {
    public:
        School(size_t actor_count);

    private:
        std::vector<Course> courses_;
        std::vector<std::shared_ptr<Actor>> actors_;
    };

} // namespace tale
#endif // TALE_SCHOOL_H