#ifndef TALE_SCHOOL_H
#define TALE_SCHOOL_H

#include <memory>
#include <vector>
#include "tale/course.hpp"
#include "tale/setting.hpp"
#include "tale/globals/random.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/actor.hpp"

namespace tale
{
    enum class Weekday
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };
    static const char *weekday_string[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    /**
     * @brief School class that is responsible for managing actors and courses during the simulation
     *
     * The School class uses a \link Setting Settings \endlink object in its constructor to setup the simulation, instancing the appropriate amount of \link Actor Actors \endlink and \link Course Courses \endlink and then filling said
     * \link Course Courses \endlink with \link Actor Actors \endlink. Afterwards it can be used to run the simulation.
     */
    class School
    {
    public:
        /**
         * @brief Constructor that sets up everything necessayr for the simulation.
         *
         * Stores references to a Random and an InteractionStore Object, to be used in the simulation. Uses the passed Setting object to instance actors and courses correctly.
         */
        School(Random &random, const Setting &setting, InteractionStore &interaction_store);

        /**
         * @brief Runs the simulation for the passed amount of days.
         *
         * Calls the private SimulatedDay function and increases current_day_ and increments current_weekday_.
         */
        void SimulateDays(size_t days);

    private:
        /**
         * @brief Holds all instanced courses
         */
        std::vector<Course> courses_;
        /**
         * @brief Holds all instanced actors.
         */
        std::vector<std::shared_ptr<Actor>> actors_;
        /**
         * @brief Holds a reference to an instance of the Random object that was passed during construction.
         */
        Random &random_;
        /**
         * @brief Holds a reference to an instance of the Setting object that was passed during construction.
         */
        const Setting &setting_;
        /**
         * @brief Holds a reference to an instance of the InteractionStore object that was passed during construction.
         */
        InteractionStore &interaction_store_;
        /**
         * @brief The current tick of the simulation (increases after each slot or freetime activity has commenced for every actor). This is always the tick that will be simulated next.
         */
        size_t current_tick = 0;
        /**
         * @brief The current day. This is always the day that will be simulated next.
         */
        size_t current_day_ = 0;
        /**
         * @brief The current weekday. This is always the weekday that will be simulated next.
         */
        Weekday current_weekday_ = Weekday::Monday;
        /**
         * @brief Simulates a single day.
         *
         * Depending on wether it is a workday or the weekend let's the actors go through different \link Interaction Interactions \endlink.
         */
        void SimulateDay(size_t day, Weekday weekday);
        /**
         * @brief Simulates a tick where the  \link Actor Actors \endlink have free time.
         */
        void FreeTimeTick();
        /**
         * @brief Checks wheter the passed Weeekday is a workday or not.
         *
         * @return The result of the check.
         */
        bool IsWorkday(Weekday weekday) const;
        /**
         * @brief Checks wheter the passed Actor is in the passed course group.
         *
         * @return The result of the check.
         */
        bool ActorIsInCourseGroup(const std::shared_ptr<Actor> &actor, const std::vector<std::weak_ptr<Actor>> &course_group) const;
        /**
         * @brief Transforms a weekday and a daily tick to a slot index.
         *
         * Slots increase each day. So if we assume there are 6 slots per day, the slot indices for monday are 0, 1, 2, 3, 4, 5 and the slots for tuesday 6, 7, 8, 9, 10, 11.
         * This transforms a weekday and daily tick index (which in the example above would go from 0-5) to corresponding slot index.
         *
         * @return The slot index.
         */
        size_t WeekdayAndDailyTickToSlot(Weekday weekday, size_t tick) const;
        /**
         * @brief Finds a random group of \link Actor Actors \endlink that could take the Course during those slots.
         *
         * Picks random \link Actor Actors \endlink and checks if they do not already have the Course corresponding to the course_id. It furhter makes sure that the \link Actor Actors \endlink
         * also are free during the passed slots. Because of those conditions it is not guaranteed that a course_group will have the full amount of \link Actor Actors \endlink specified in setting_.
         *
         * @return The found group of Actors.
         */
        std::vector<std::weak_ptr<Actor>> FindRandomCourseGroup(size_t course_id, const std::vector<uint32_t> &slots);
    };

} // namespace tale
#endif // TALE_SCHOOL_H