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
     * @brief A Course grouping \link Actor Actors \endlink together during different slots of the day.
     *
     * A Course happens a few times week. Those times are called slots. Each slots has a fixed group of \link Actor Actors \endlink that visit the Course
     * during that time. As \link Actor Actors \endlink can have the same Course a few times each week, some slots feature the same group of \link Actor Actors \endlink but it is
     * guaranteed that \link Actor Actors \endlink always visit a single Course in the same group.
     */
    class Course
    {
    public:
        /**
         * @brief Name of the Course.
         */
        const std::string name_;
        /**
         * @brief Id of the Course. Should correspond to the index of it in the vector holding all courses in an School object.
         */
        const size_t id_;
        /**
         * @brief Constructor that uses the passed Setting to setup the course.
         *
         * In addition to seting up the slots_ vector depending on the Setting object, the Constructor stores a reference to the passed Random object and applies the name.
         */
        Course(Random &random, const Setting &setting, size_t id, std::string name_);
        /**
         * @brief Finds the group of \link Actor Actors \endlink for the passed slot.
         *
         * Looks in the slot_ vector for the group of \link Actor Actors \endlink that visit the course during the passed slot.
         * This can crash if the slot is not filled.
         *
         * @return The found group.
         */
        const std::vector<std::weak_ptr<Actor>> &GetCourseGroupForSlot(size_t slot);
        /**
         * @brief Checks if the course has all slots filled.
         *
         *
         * @return The result of the check.
         */
        bool AllSlotsFilled() const;
        /**
         * @brief Looks for a random empty slot.
         *
         * This can crash if there is no empty slot.
         *
         * @return The found slot.
         */
        uint32_t GetRandomEmptySlot() const;
        /**
         * @brief Adds the passed group of \link Actor Actors \endlink to the passed slot.
         *
         * This can crash if slot is not empty.
         */
        void AddToSlot(std::vector<std::weak_ptr<Actor>> actors, size_t slot);

    private:
        /**
         * @brief Holds the group of \link Actor Actors \endlink for each slot.
         */
        std::vector<std::vector<std::weak_ptr<Actor>>> slots_;
        /**
         * @brief Holds a reference to the Randmo objec that was passed during construction.
         */
        Random &random_;
    };

} // namespace tale
#endif // TALE_COURSE_H