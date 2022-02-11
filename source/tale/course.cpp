#include <iostream>
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
    void Course::TickSlot(size_t slot)
    {
        std::cout << "TICKING SLOT " << slot << " OF COURSE " << id_ << std::endl;
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