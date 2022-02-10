#include <algorithm>
#include "tale/school.hpp"

namespace tale
{
    School::School(size_t actor_count)
    {
        for (size_t i = 0; i < actor_count; ++i)
        {
            std::shared_ptr<Actor> actor(new Actor());
            actors_.push_back(actor);
        }
        // TODO: add as parameter
        size_t actors_per_class = 30;
        // 8 different courses are necessary to completely fill an actors schedule, assuming he has that course 4 times a week
        // and there are 6 courses per day (in a 5 day workweek)
        size_t min_course_count = 8;
        size_t course_count = std::max(actor_count / actors_per_class, min_course_count);
        for (size_t i = 0; i < course_count; ++i)
        {
            Course course(i, "Course" + std::to_string(i));
            courses_.push_back(course);
        }
        // Pseudocode:
        //  for course in all courses (min 8)
        //      until all 30 slot are filled
        //          get 30 random actors (check if they are still free and dont have this class)
        //          assign to 4 random slots
    }
} // namespace tale