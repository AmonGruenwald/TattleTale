#ifndef TALE_COURSE_H
#define TALE_COURSE_H

#include "tale/globals/random.hpp"
#include "tale/actor.hpp"
#include "tale/setting.hpp"
#include <string>
#include <memory>

namespace tale
{
    /**
     * @brief Course managing actors
     */
    class Course
    {
    public:
        const std::string name_;
        const size_t id_;
        Course(Random &random, const Setting &setting, size_t id, std::string name_);
        const std::vector<std::weak_ptr<Actor>> &GetCourseGroupForSlot(size_t slot);
        bool AllSlotsFilled() const;
        int GetRandomEmptySlot() const;
        void AddToSlot(std::vector<std::weak_ptr<Actor>> actors, size_t slot);

    private:
        std::vector<std::vector<std::weak_ptr<Actor>>> slots_;
        Random &random_;
    };

} // namespace tale
#endif // TALE_COURSE_H