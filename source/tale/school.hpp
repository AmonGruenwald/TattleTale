#ifndef TALE_SCHOOL_H
#define TALE_SCHOOL_H

#include <memory>
#include <vector>
#include "tale/course.hpp"
#include "tale/actor.hpp"
#include "tale/globals/random.hpp"

namespace tale
{
    /**
     * @brief School managing actors and courses
     */
    class School
    {
    public:
        School(Random &random, size_t actor_count);

    private:
        std::vector<std::weak_ptr<Actor>> FindRandomCourseGroup(size_t course_id, size_t actor_count);
        std::vector<Course> courses_;
        std::vector<std::shared_ptr<Actor>> actors_;
        Random &random_;
    };

} // namespace tale
#endif // TALE_SCHOOL_H