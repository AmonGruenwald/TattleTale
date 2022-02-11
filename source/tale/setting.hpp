#ifndef TALE_SETTING_H
#define TALE_SETTING_H

#include <math.h>
#include <algorithm>
namespace tale
{
    /**
     * @brief Stores all settings necessary for the simulation
     */
    struct Setting
    {
        size_t days_to_simulate = 30;
        size_t actor_count = 300;
        size_t courses_per_day = 6;
        size_t actors_per_course = 30;
        size_t same_course_per_week = 4;
        // 8 different courses are necessary to completely fill an actors schedule, assuming he has that course 4 times a week
        // and there are 6 courses per day (in a 5 day workweek)
        size_t slot_count_per_week() const
        {
            // 5 day workweek
            return courses_per_day * 5;
        }
        /**
         * @brief Calculates the minimum necessary amount of courses so every actor can completely feel their schedule
         *
         * @return The amount of courses
         */
        size_t minimum_necessary_course_count() const
        {
            return (size_t)ceil((double)slot_count_per_week() / (double)same_course_per_week);
        }
        size_t course_count() const
        {
            return std::max((size_t)minimum_necessary_course_count(), (size_t)ceil((double)actor_count / (double)actors_per_course));
        }
    };

} // namespace tale
#endif // TALE_SETTING_H