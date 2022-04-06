#ifndef TALE_COURSE_H
#define TALE_COURSE_H

#include "shared/random.hpp"
#include "shared/actor.hpp"
#include "shared/setting.hpp"
#include <string>
#include <memory>
#include <list>

namespace tattletale
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
         * @param random Reference to Random object of the simulation
         * @param setting Reference to the Setting object of the simulation
         * @param name The name of the Course
         */
        Course(Random &random, const Setting &setting, size_t id, std::string name);
        /**
         * @brief Finds the group of \link Actor Actors \endlink for the passed slot.
         *
         * Looks in the slot_ vector for the group of \link Actor Actors \endlink that visit the course during the passed slot.
         * This can crash if the slot is not filled.
         *
         * @param slot The slot where the group of \link Actor Actors \endlink will be looked up.
         * @return The found group.
         */
        const std::list<Actor *> &GetCourseGroupForSlot(size_t slot);
        /**
         * @brief Checks wether there is still space left in a slot.
         *
         * @param slot The slot we want to check.
         * @return The result of the check.
         */
        bool SpaceInSlot(size_t slot) const;
        /**
         * @brief Checks if the course has all slots completely filled.
         *
         *
         * @return The result of the check.
         */
        bool AllSlotsFilled() const;
        /**
         * @brief Checks if the course has at least one Actor in every slot.
         *
         *
         * @return The result of the check.
         */
        bool AllSlotsNotEmpty() const;
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
         *
         * @param actors List of \link Actor Actors \endlink that will be added to the slot.
         * @param slot The slot the \link Actor Actors \endlink will be added to.
         */
        void AddToSlot(std::list<Actor *> actors, size_t slot);
        /**
         * @brief Adds the Actor to the passed slot.
         *
         * This can crash if slot is not empty.
         *
         * @param actor Actor that will be added to the slot.
         * @param slot The slot the Actor will be added to.
         */
        void AddToSlot(Actor *actor, size_t slot);
        /**
         * @brief Clears the \link Actor Actors \endlink out of a slot.
         *
         * This can crash if slot is already empty.
         *
         * @param slot The slot the \link Actor Actors \endlink will be cleared from.
         * @return The group of Actors that was cleared out of the slot.
         */
        std::list<Actor *> ClearSlot(size_t slot);
        /**
         * @brief Getter for the amount of slots the Course currenty holds.
         *
         * @return The amount of slots.
         */
        size_t GetSlotCount() const;
        /**
         * @brief Getter for the amount of \link Actor Actors \endlink  the Course holds in the passed slot.
         *
         * @param slot_index The index of the slot to check.
         * @return The amount of \link Actor Actors \endlink in the slot.
         */
        size_t GetActorCount(size_t slot_index) const;

    private:
        /**
         * @brief Holds the group of \link Actor Actors \endlink for each slot.
         */
        std::vector<std::list<Actor *>> slots_;
        /**
         * @brief Holds a reference to the Random object that was passed during construction.
         */
        Random &random_;
        /**
         * @brief Holds a reference to the Setting object that was passed during construction.
         */
        const Setting &setting_;
    };

} // namespace tattletale

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::Course, T>::value, char>> : fmt::formatter<std::string>
{
    // d - default
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'd'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(tattletale::Course &course, FormatContext &ctx) -> decltype(ctx.out())
    {
        std::string description = fmt::format("{}:\n", course.name_);

        size_t count = 0;
        for (size_t slot_index = 0; slot_index < course.GetSlotCount(); ++slot_index)
        {
            if (count == 4)
            {
                count = 0;
                description += "\n";
            }
            ++count;
            description += fmt::format("[{:02}]", course.GetActorCount(slot_index));
        }
        return description;
        return fmt::formatter<std::string>::format(description, ctx);
    }
};
#endif // TALE_COURSE_H