#include <algorithm>
#include <iostream>
#include "tale/school.hpp"

namespace tale
{
    School::School(Random &random, size_t actor_count) : random_(random)
    {
        for (size_t i = 0; i < actor_count; ++i)
        {
            std::shared_ptr<Actor> actor(new Actor());
            actor->name_ = (std::to_string(i) + " ");
            actors_.push_back(actor);
        }
        actors_[0]->EnrollInCourse(0);
        // TODO: add as a parameter
        size_t actors_per_class = 30;
        // 8 different courses are necessary to completely fill an actors schedule, assuming he has that course 4 times a week
        // and there are 6 courses per day (in a 5 day workweek)
        size_t min_course_count = 8;
        size_t course_count = std::max(actor_count / actors_per_class, min_course_count);
        for (size_t i = 0; i < course_count; ++i)
        {
            Course course(random_, i, "Course" + std::to_string(i));
            courses_.push_back(course);
        }
        // Pseudocode:
        //  for course in all courses (min 8)
        //      until all 30 slot are filled
        //          get 30 random actors (check if they are still free and dont have this course)
        //          assign to 4 random slots

        for (auto &course : courses_)
        {
            uint16_t group_used = 0;
            std::vector<std::weak_ptr<Actor>> course_group = FindRandomCourseGroup(course.id_, actors_per_class);
            while (!course.AllSlotsFilled())
            {

                // TODO: add as a parameter
                uint16_t same_course_per_week = 4;
                if (group_used == same_course_per_week)
                {
                    course_group = FindRandomCourseGroup(course.id_, actors_per_class);
                    group_used = 0;
                }
                ++group_used;
                uint32_t slot = course.AddToRandomEmptySlot(course_group);
                if (slot == -1)
                {
                    break;
                }
                std::cout << "Slot " << slot << " of course " << course.id_ << " filled with " << course_group.size() << " actors: ";
                for (auto &actor : course_group)
                {
                    std::cout << actor.lock()->name_;
                }
                std::cout << std::endl;
            }
        }
    }

    void School::SimulateDays(size_t days)
    {
        for (size_t i = 0; i < days; ++i)
        {
            SimulateDay(current_day_, current_weekday_);
            ++current_day_;
            current_weekday_ = static_cast<Weekday>((static_cast<int>(current_weekday_) + 1) % 7);
        }
    }

    void School::SimulateDay(size_t day, Weekday weekday)
    {
        // TODO: add as parameter
        size_t courses_per_day = 6;
        std::cout << "SIMULATING DAY " << day << " WHICH IS A " << weekday_string[static_cast<int>(weekday)] << std::endl;
        if (IsWorkday(weekday))
        {
            for (size_t i = 0; i < courses_per_day; ++i)
            {
                size_t slot = WeekdayTickToSlot(weekday, i);
                for (auto &course : courses_)
                {
                    course.TickSlot(slot);
                }
                ++current_tick;
            }
            FreeTimeTick();
        }
        else
        {
            for (size_t i = 0; i < courses_per_day + 1; ++i)
            {
                FreeTimeTick();
            }
        }
    }
    void School::FreeTimeTick()
    {
        std::cout << "FREETIME TICK " << std::endl;
        ++current_tick;
    }
    bool School::IsWorkday(Weekday weekday)
    {
        if (weekday == Weekday::Saturday || weekday == Weekday::Sunday)
        {
            return false;
        }
        return true;
    }

    size_t School::WeekdayTickToSlot(Weekday weekday, size_t tick)
    {
        // TODO: add as parameter
        size_t courses_per_day = 6;
        return static_cast<size_t>(weekday) * courses_per_day + tick;
    }

    std::vector<std::weak_ptr<Actor>> School::FindRandomCourseGroup(size_t course_id, size_t actor_count)
    {
        std::vector<std::weak_ptr<Actor>> course_group;
        size_t current_try = 0;
        while (course_group.size() < actor_count && current_try < actor_count)
        {
            size_t random_actor_index = random_.GetUInt(0, actors_.size() - 1);

            size_t current_index_search_try = 0;
            while (actors_[random_actor_index]->IsEnrolledInCourse(course_id) && current_index_search_try < actors_.size())
            {
                random_actor_index += -1;
                random_actor_index %= actors_.size();
                ++current_index_search_try;
            }
            if (current_index_search_try != actors_.size())
            {
                actors_[random_actor_index]->EnrollInCourse(course_id);
                course_group.push_back(actors_[random_actor_index]);
            }
            ++current_try;
        }
        return course_group;
    }
} // namespace tale