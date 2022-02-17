#include "tale/talecore.hpp"
#include "tale/school.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>

namespace tale
{
    School::School(const Setting &setting) : setting_(setting)
    {
        random_ = Random(setting_.seed);
        interaction_store_ = InteractionStore();
        size_t actor_count = setting_.actor_count;
        for (size_t i = 0; i < actor_count; ++i)
        {
            TALE_DEBUG_PRINT("CREATING ACTOR " + std::to_string(i) + "\n");
            std::shared_ptr<Actor> actor(new Actor(*this, i));
            // TODO: not needed after random names are used
            actor->name_ = (std::to_string(i) + " ");
            actors_.push_back(actor);
        }

        size_t course_count = setting_.course_count();
        for (size_t i = 0; i < course_count; ++i)
        {
            TALE_DEBUG_PRINT("CREATING COURSE " + std::to_string(i) + "\n");
            // TODO: create better names
            Course course(random_, setting_, i, "Course" + std::to_string(i));
            courses_.push_back(course);
        }

        size_t slot_count_per_week = setting_.slot_count_per_week();
        std::vector<uint32_t> random_slot_order;
        for (auto &course : courses_)
        {
            TALE_DEBUG_PRINT("FILLING COURSE " + std::to_string(course.id_) + "|n");
            random_slot_order.clear();
            // randomly order all slots of the course
            // TODO: optimize this by shuffling an already filled vector everytime instead of this
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
            // Go over everyslot
            while (slot_index < random_slot_order.size() && setting_.same_course_per_week > 0)
            {
                std::vector<uint32_t> slots;
                // Because Actor have the same course a few times per week use the same group a few times
                for (size_t i = 0; i < setting_.same_course_per_week && slot_index < random_slot_order.size(); ++i)
                {
                    slots.push_back(random_slot_order[slot_index]);
                    ++slot_index;
                }
                std::vector<std::weak_ptr<Actor>> course_group = FindRandomCourseGroup(course.id_, slots);

                TALE_DEBUG_PRINT("\tSLOTS ");
                for (size_t i = 0; i < slots.size(); ++i)
                {
                    TALE_DEBUG_PRINT(slots[i]);
                    if (i != slots.size() - 1)
                    {
                        TALE_DEBUG_PRINT(",");
                    }
                    TALE_DEBUG_PRINT(" ");
                    course.AddToSlot(course_group, slots[i]);
                }

                TALE_DEBUG_PRINT("FILLED WITH " + std::to_string(course_group.size()) + " ACTORS.\n");
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
    std::weak_ptr<Actor> School::GetActor(size_t actor_id)
    {
        assert(actor_id < actors_.size());
        return actors_[actor_id];
    }
    Course &School::GetCourse(size_t course_id)
    {
        assert(course_id < courses_.size());
        return courses_[course_id];
    }

    size_t School::GetCurrentDay() const
    {
        return current_day_;
    }
    Weekday School::GetCurrentWeekday() const
    {
        return current_weekday_;
    }
    const Setting &School::GetSetting() const
    {
        return setting_;
    }
    InteractionStore &School::GetInteractionStore()
    {
        return interaction_store_;
    }
    Random &School::GetRandom()
    {
        return random_;
    }
    void School::SimulateDay(size_t day, Weekday weekday)
    {
        TALE_DEBUG_PRINT("SIMULATING DAY " + std::to_string(day) + " WHICH IS A " + weekday_string[static_cast<int>(weekday)] + "\n");
        if (IsWorkday(weekday))
        {
            for (size_t i = 0; i < setting_.courses_per_day; ++i)
            {
                size_t slot = WeekdayAndDailyTickToSlot(weekday, i);
                for (auto &course : courses_)
                {
                    std::vector<std::weak_ptr<Actor>> course_group = course.GetCourseGroupForSlot(slot);
                    for (auto &actor : course_group)
                    {
                        std::vector<std::weak_ptr<Kernel>> reasons;
                        std::vector<std::weak_ptr<Actor>> participants;
                        std::string interaction_name = actor.lock()->ChooseInteraction(course_group, reasons, participants);
                        // TODO: registers interaction to events
                        std::shared_ptr<Interaction> interaction = interaction_store_.CreateInteraction(interaction_name, current_tick, reasons, participants);
                        interaction->Apply();
                    }
                }
                ++current_tick;
            }
            FreeTimeTick();
            ++current_tick;
        }
        else
        {
            for (size_t i = 0; i < setting_.courses_per_day + 1; ++i)
            {
                FreeTimeTick();
                ++current_tick;
            }
        }
    }

    void School::FreeTimeTick()
    {
        TALE_DEBUG_PRINT("FREETIME TICK\n");
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

    size_t School::WeekdayAndDailyTickToSlot(Weekday weekday, size_t daily_tick) const
    {
        return static_cast<size_t>(weekday) * setting_.courses_per_day + daily_tick;
    }

    std::vector<std::weak_ptr<Actor>> School::FindRandomCourseGroup(size_t course_id, const std::vector<uint32_t> &slots)
    {
        std::vector<std::weak_ptr<Actor>> course_group;
        if (actors_.size() > 0)
        {
            for (size_t i = 0; i < setting_.actors_per_course; ++i)
            {
                size_t random_actor_index = random_.GetUInt(0, actors_.size() - 1);

                size_t current_index_search_try = 0;
                while (
                    (actors_[random_actor_index]->AllSlotsFilled() || actors_[random_actor_index]->IsEnrolledInCourse(course_id) || ActorIsInCourseGroup(actors_[random_actor_index], course_group) || !actors_[random_actor_index]->SlotsEmpty(slots)) && current_index_search_try < setting_.actor_count)
                {
                    random_actor_index += -1;
                    random_actor_index %= actors_.size();
                    ++current_index_search_try;
                }
                if (current_index_search_try != actors_.size())
                {
                    course_group.push_back(actors_[random_actor_index]);
                }
            }
        }
        return course_group;
    }
} // namespace tale