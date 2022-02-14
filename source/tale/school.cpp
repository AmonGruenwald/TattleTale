#include <algorithm>
#include <iostream>
#include "tale/school.hpp"

namespace tale
{
    School::School(Random &random, const Setting &setting, InteractionStore &interaction_store) : random_(random), setting_(setting), interaction_store_(interaction_store)
    {
        for (size_t i = 0; i < setting_.actor_count; ++i)
        {
            std::shared_ptr<Actor> actor(new Actor(random, setting));
            actor->name_ = (std::to_string(i) + " ");
            actors_.push_back(actor);
        }
        size_t course_count = setting_.course_count();
        for (size_t i = 0; i < course_count; ++i)
        {
            // TODO: create better names
            Course course(random_, setting_, i, "Course" + std::to_string(i));
            courses_.push_back(course);
        }

        for (auto &course : courses_)
        {
            std::vector<uint32_t> random_slot_order;
            size_t slot_count_per_week = setting_.slot_count_per_week();
            while (random_slot_order.size() < slot_count_per_week)
            {
                uint32_t random_slot = random_.GetUInt(0, slot_count_per_week - 1);
                while (std::count(random_slot_order.begin(), random_slot_order.end(), random_slot))
                {
                    random_slot += 1;
                    random_slot %= slot_count_per_week;
                }
                random_slot_order.push_back(random_slot);
            }

            size_t slot_index = 0;
            while (slot_index < random_slot_order.size())
            {
                std::vector<uint32_t> slots;
                for (size_t i = 0; i < setting_.same_course_per_week && slot_index < random_slot_order.size(); ++i)
                {
                    slots.push_back(random_slot_order[slot_index]);
                    ++slot_index;
                }
                std::vector<std::weak_ptr<Actor>> course_group = FindRandomCourseGroup(course.id_, slots);

                for (size_t i = 0; i < slots.size(); ++i)
                {
                    course.AddToSlot(course_group, slots[i]);
                }
            }
        }
        for (auto &actor : actors_)
        {
            std::cout << actor->name_ << "is enrolled in " << actor->GetFilledSlotsCount() << " Courses." << std::endl;
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
        std::cout << "SIMULATING DAY " << day << " WHICH IS A " << weekday_string[static_cast<int>(weekday)] << std::endl;
        if (IsWorkday(weekday))
        {
            for (size_t i = 0; i < setting_.courses_per_day; ++i)
            {
                size_t slot = WeekdayAndDailyTickToSlot(weekday, i);
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
            for (size_t i = 0; i < setting_.courses_per_day + 1; ++i)
            {
                FreeTimeTick();
            }
        }
    }
    void School::FreeTimeTick()
    {
        // std::cout << "FREETIME TICK " << std::endl;
        ++current_tick;
    }
    bool School::IsWorkday(Weekday weekday) const
    {
        if (weekday == Weekday::Saturday || weekday == Weekday::Sunday)
        {
            return false;
        }
        return true;
    }

    bool School::ActorIsInCourseGroup(const std::shared_ptr<Actor> &actor, const std::vector<std::weak_ptr<Actor>> &course_group) const
    {
        for (auto &actor_in_course_group : course_group)
        {
            if (actor_in_course_group.lock() == actor)
            {
                return true;
            }
        }
        return false;
    }

    size_t School::WeekdayAndDailyTickToSlot(Weekday weekday, size_t tick) const
    {
        return static_cast<size_t>(weekday) * setting_.courses_per_day + tick;
    }

    std::vector<std::weak_ptr<Actor>> School::FindRandomCourseGroup(size_t course_id, const std::vector<uint32_t> &slots)
    {
        std::vector<std::weak_ptr<Actor>> course_group;
        size_t current_try = 0;
        while (course_group.size() < setting_.actor_count && current_try < setting_.actor_count)
        {
            size_t random_actor_index = random_.GetUInt(0, actors_.size() - 1);

            size_t current_index_search_try = 0;
            while (
                (actors_[random_actor_index]->AllSlotsFilled() || actors_[random_actor_index]->IsEnrolledInCourse(course_id) || ActorIsInCourseGroup(actors_[random_actor_index], course_group) || !actors_[random_actor_index]->SlotsEmpty(slots)) && current_index_search_try < actors_.size())
            {
                random_actor_index += -1;
                random_actor_index %= actors_.size();
                ++current_index_search_try;
            }
            if (current_index_search_try != actors_.size())
            {
                course_group.push_back(actors_[random_actor_index]);
            }
            ++current_try;
        }
        return course_group;
    }
} // namespace tale