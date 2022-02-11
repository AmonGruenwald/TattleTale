#ifndef TALE_SCHOOL_H
#define TALE_SCHOOL_H

#include <memory>
#include <vector>
#include "tale/course.hpp"
#include "tale/actor.hpp"
#include "tale/setting.hpp"
#include "tale/globals/random.hpp"

namespace tale
{
    enum class Weekday
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };
    static const char *weekday_string[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    /**
     * @brief School managing actors and courses
     */
    class School
    {
    public:
        School(Random &random, const Setting &setting);
        void SimulateDays(size_t days);

    private:
        void SimulateDay(size_t day, Weekday weekday);
        void FreeTimeTick();
        bool IsWorkday(Weekday weekday);
        size_t WeekdayAndTickToSlot(Weekday weekday, size_t tick);
        std::vector<std::weak_ptr<Actor>> FindRandomCourseGroup(size_t course_id, size_t actor_count);
        std::vector<Course> courses_;
        std::vector<std::shared_ptr<Actor>> actors_;
        Random &random_;
        const Setting &setting_;
        size_t current_tick = 0;
        size_t current_day_ = 0;
        Weekday current_weekday_ = Weekday::Monday;
    };

} // namespace tale
#endif // TALE_SCHOOL_H