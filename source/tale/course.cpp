#include "tale/course.hpp"

namespace tale
{
    Course::Course(Random &random, size_t id, std::string name) : id_(id), name_(name), random_(random)
    {
        // TODO: add as a parameter depenend on amount of courses per day
        size_t slot_count = 30;
        for (size_t i = 0; i < slot_count; ++i)
        {
            slots_.push_back(std::vector<std::weak_ptr<Actor>>());
        }
    }

    bool Course::AllSlotsFilled()
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
    int Course::AddToRandomEmptySlot(std::vector<std::weak_ptr<Actor>> actors)
    {
        uint32_t random_slot = random_.GetUInt(0, slots_.size() - 1);
        size_t current_try = 0;
        while (slots_[random_slot].size() != 0 && current_try <= slots_.size() + 1)
        {
            ++current_try;
            random_slot += 1;
            random_slot %= slots_.size();
        }
        slots_[random_slot] = actors;
        if (current_try <= slots_.size() + 1)
        {

            return random_slot;
        }
        else
        {
            return -1;
        }
    }
} // namespace tale