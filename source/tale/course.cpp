#include <iostream>
#include <assert.h>
#include "tale/course.hpp"

namespace tale
{
    Course::Course(Random &random, const Setting &setting, size_t id, std::string name) : id_(id), name_(name), random_(random)
    {
        size_t slot_count_per_week = setting.slot_count_per_week();
        for (size_t i = 0; i < slot_count_per_week; ++i)
        {
            slots_.push_back(std::vector<std::weak_ptr<Actor>>());
        }
    }

    const std::vector<std::weak_ptr<Actor>> &Course::GetCourseGroupForSlot(size_t slot)
    {
        return slots_[slot];
    }

    bool Course::AllSlotsFilled() const
    {
        for (auto &slot : slots_)
        {
            if (slot.size() == 0)
            {
                return false;
            }
        }
        return true;
    }
    uint32_t Course::GetRandomEmptySlot() const
    {
        assert(!AllSlotsFilled()); // no more empty slots
        uint32_t random_slot = random_.GetUInt(0, slots_.size() - 1);
        while (slots_[random_slot].size() != 0)
        {
            random_slot += 1;
            random_slot %= slots_.size();
        }
        return random_slot;
    }
    void Course::AddToSlot(std::vector<std::weak_ptr<Actor>> actors, size_t slot)
    {
        assert(!AllSlotsFilled());        // no more empty slots
        assert(slots_[slot].size() == 0); // slot not empty
        slots_[slot] = actors;
        for (auto &actor : actors)
        {
            actor.lock()->EnrollInCourse(id_, slot);
        }
    }
} // namespace tale