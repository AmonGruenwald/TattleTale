#include <iostream>
#include <assert.h>
#include "tale/course.hpp"
#include <fmt/core.h>

namespace tattletale
{
    Course::Course(Random &random, const Setting &setting, size_t id, std::string name) : id_(id), name_(name), random_(random), setting_(setting)
    {
        size_t slot_count_per_week = setting.slot_count_per_week();
        for (size_t i = 0; i < slot_count_per_week; ++i)
        {
            slots_.push_back(std::vector<Actor *>());
        }
    }

    const std::vector<Actor *> &Course::GetCourseGroupForSlot(size_t slot)
    {
        return slots_[slot];
    }
    bool Course::SpaceInSlot(size_t slot) const
    {
        return (slots_[slot].size() < setting_.actors_per_course);
    }
    bool Course::AllSlotsFilled() const
    {
        for (auto &slot : slots_)
        {
            if (slot.size() < setting_.actors_per_course)
            {
                return false;
            }
        }
        return true;
    }
    uint32_t Course::GetRandomEmptySlot() const
    {
        // TOOD as AllSlotsFilled is also false if slots are only partially filled this assert does not stop every misuse

        TATTLETALE_ERROR_PRINT(!AllSlotsFilled(), "All slots are filled.");
        uint32_t random_slot = random_.GetUInt(0, slots_.size() - 1);
        while (slots_[random_slot].size() != 0)
        {
            random_slot += 1;
            random_slot %= slots_.size();
        }
        return random_slot;
    }
    void Course::AddToSlot(std::vector<Actor *> actors, size_t slot)
    {
        TATTLETALE_ERROR_PRINT(slots_[slot].size() + actors.size() <= setting_.actors_per_course, fmt::format("Slot with id {} does not have enoug space", slot));

        for (auto &actor : actors)
        {
            slots_[slot].push_back(actor);
            actor->EnrollInCourse(id_, slot);
        }
    }
    void Course::AddToSlot(Actor *actor, size_t slot)
    {
        TATTLETALE_ERROR_PRINT(slots_[slot].size() < setting_.actors_per_course, fmt::format("Slot with id {} does not have enoug space", slot));
        slots_[slot].push_back(actor);
        actor->EnrollInCourse(id_, slot);
    }

    std::vector<Actor *> Course::ClearSlot(size_t slot)
    {
        TATTLETALE_ERROR_PRINT(slots_[slot].size() != 0, fmt::format("Slot with id {} is already empty", slot));
        std::vector<Actor *> group = slots_[slot];
        slots_[slot].clear();
        for (auto &actor : group)
        {
            actor->EjectFromCourse(id_, slot);
        }
        return group;
    }

    size_t Course::GetSlotCount() const
    {
        return slots_.size();
    }

    size_t Course::GetActorCount(size_t slot_index) const
    {
        return slots_[slot_index].size();
    }
    std::string Course::ToString() const
    {
        std::string description = name_ + ":\n";

        size_t count = 0;
        for (auto &slot : slots_)
        {
            if (count == 4)
            {
                count = 0;
                description += "\n";
            }
            ++count;
            description += fmt::format("[{:02}]", slot.size());
        }
        return description;
    }
} // namespace tattletale